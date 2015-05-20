#include <algorithm>

#include "../MyEngine/MainEngine/Random.h"
#include "../MyEngine/MainEngine/ResourceManager.h"

#include "TTTBoard.h"

TTTBoard::TTTBoard() {
}

TTTBoard::~TTTBoard() {
    m_winningCells.clear();
}

TTTBoard* TTTBoard::makeMove(int cell) {
    TTTBoard* newBoard = new TTTBoard(*this);
    if (newBoard->m_cells[cell] == EMPTY && m_winner == EMPTY) {
        newBoard->m_cells[cell] = m_currPlayer;
        newBoard->switchTurn();
        newBoard->checkVictory();
    }
    return newBoard;
}

Cell TTTBoard::getTurn() const {
    if (m_winner != EMPTY || std::count(m_cells, m_cells+9, EMPTY) == 0) {
        return EMPTY;
    } else {
        return m_currPlayer;
    }
}

bool TTTBoard::isEmpty() const {
    return (std::count(m_cells, m_cells+9, EMPTY) == 9);
}

int TTTBoard::getClickedCell(crvec2 mouse) const {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if ((mouse.x >= -75+j*50 && mouse.x <= -25+j*50) && (mouse.y >= 25-i*50 && mouse.y <= 75-i*50)) {
                return i*3 + j;
            }
        }
    }
    return -1;
}

Cell TTTBoard::checkVictory() {
    for (int i = 0; i < 3; i++) {
        if (m_cells[i] == m_cells[i+3] && m_cells[i+3] == m_cells[i+6] && m_cells[i+6] != EMPTY) {
            m_winningCells = {i,i+3,i+6};
            m_winner = m_cells[i];
            return m_winner;
        } else if (m_cells[i*3] == m_cells[i*3+1] && m_cells[i*3+1] == m_cells[i*3+2] && m_cells[i*3+2] != EMPTY) {
            m_winningCells = {i*3,i*3+1,i*3+2};
            m_winner = m_cells[i*3];
            return m_winner;
        }
    }
    if (m_cells[0] == m_cells[4] && m_cells[4] == m_cells[8] && m_cells[8] != EMPTY) {
        m_winningCells = {0,4,8};
        m_winner = m_cells[0];
        return m_winner;
    } else if (m_cells[2] == m_cells[4] && m_cells[4] == m_cells[6] && m_cells[6] != EMPTY) {
        m_winningCells = {2,4,6};
        m_winner = m_cells[2];
        return m_winner;
    }
    m_winner = EMPTY;
    return EMPTY;
}

int TTTBoard::getNumWaysToWin() const {
    int numWays = 0;
    for (int i = 0; i < 3; i++) {
        if (m_cells[i] + m_cells[i+3] + m_cells[i+6] == EMPTY+2*m_currPlayer) {
            numWays++;
        } else if (m_cells[i*3] + m_cells[i*3+1] + m_cells[i*3+2] == EMPTY+2*m_currPlayer) {
            numWays++;
        }
    }
    if (m_cells[0] + m_cells[4] + m_cells[8] == EMPTY+2*m_currPlayer) {
        numWays++;
    } else if (m_cells[2] + m_cells[4] + m_cells[6] == EMPTY+2*m_currPlayer) {
        numWays++;
    }
    return numWays;
}

int TTTBoard::getNumMoves() const {
    return 9;
}

void TTTBoard::reset() {
    m_currPlayer = nta::Random::randBool() ? PLAYER : COMPUTER;
    m_winningCells = {0,0,0};
    m_winner = EMPTY;
    for (auto& cell : m_cells) {
        cell = EMPTY;
    }
}

void TTTBoard::switchTurn() {
    m_currPlayer = (m_currPlayer == PLAYER) ? COMPUTER : PLAYER;
}

void TTTBoard::draw(nta::PrimBatch& pbatch) const {
    static GLuint id = nta::ResourceManager::getTexture("solid.png").id;
    for (int i = 0; i < 2; i++) {
        pbatch.addPrimitive({{glm::vec2(-25+50*i,75)},{glm::vec2(-25+50*i,-75)}},id);
        pbatch.addPrimitive({{glm::vec2(-75,25-50*i)},{glm::vec2(75,25-50*i)}},id);
    }
}

void TTTBoard::drawText(nta::SpriteBatch& sbatch, nta::SpriteFont* font) const {
    std::string text = toString((char)m_currPlayer) + "'s Turn";
    if (m_winner != EMPTY) {
        text = toString((char)m_winner) + " Won";
    }
    font->drawText(sbatch, text, glm::vec4(-80,98,160,21));
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            glm::vec4 col(1);
            if (m_winner != EMPTY && std::find(m_winningCells.begin(), m_winningCells.end(), i*3+j) != m_winningCells.end()) {
                col = glm::vec4(0,1,.2,1);
            }
            font->drawText(sbatch, toString((char)m_cells[i*3+j]), glm::vec4(-70+50*j,70-50*i,40,40), col);
        }
    }
}

MMNode* TTTBoard::generateMove(int move) {
    return makeMove(move);
}

float TTTBoard::evaluate() const {
    float val = 0;
    switch(m_winner) {
        case PLAYER: return -10; break;
        case COMPUTER: return 10; break;
        case EMPTY: if (std::count(m_cells, m_cells+9, EMPTY) == 0) val -= 1; break;
    } switch (getNumWaysToWin()) {
        case 0: val -= 1; break;
        case 1: val += (m_currPlayer == COMPUTER) ? 5 : -5; break;
        default: val += (m_currPlayer == PLAYER) ? 8 : -8; break;
    }
    return val;
}

bool TTTBoard::same(MMNode* rhs) const {
    for (int i = 0; i < 9; i++) {
        if (m_cells[i] != ((TTTBoard*)rhs)->m_cells[i]) {
            return false;
        }
    }
    return true;
}
