#include <algorithm>

#include "../MyEngine/MainEngine/ResourceManager.h"
#include "../MyEngine/MainEngine/Random.h"

#include "C4Board.h"

C4Board::C4Board() {
}

C4Board::~C4Board() {
}

C4Board* C4Board::makeMove(int col) const {
    C4Board* newBoard = new C4Board(*this);
    for (int i = 5; i >= 0; i--) {
        if (newBoard->m_cells[i][col] == EMPTY && inRange(col,0,6) && m_winner == EMPTY) {
            newBoard->m_cells[i][col] = m_currPlayer;
            newBoard->switchTurn();
            newBoard->checkWinner();
            return newBoard;
        }
    }
    return newBoard;
}

Cell C4Board::getTurn() const {
    if (m_winner != EMPTY || std::count(&m_cells[0][0], &m_cells[0][0]+42, EMPTY) == 0) {
        return EMPTY;
    } else {
        return m_currPlayer;
    }
}

Cell C4Board::checkWinner() {
    auto checkDirection =
        [&](int r, int c, int dr, int dc) {
            if (!inRange(r+3*dr,0,5) || !inRange(c+3*dc,0,6)) {
                return false;
            }
            for (int i = 1; i < 4; i++) {
                if (m_cells[r][c] != m_cells[r+i*dr][c+i*dc]) {
                    return false;
                }
            }
            return true;
        };

    m_winner = EMPTY;
    for (int r = 0; r < 6; r++) {
        for (int c = 0; c < 7; c++) {
            if (m_cells[r][c] == EMPTY) {
                continue;
            } else if (checkDirection(r,c,0,1)) {
                m_winner = m_cells[r][c];
                return m_cells[r][c];
            } else if (checkDirection(r,c,1,1)) {
                m_winner = m_cells[r][c];
                return m_cells[r][c];
            } else if (checkDirection(r,c,1,0)) {
                m_winner = m_cells[r][c];
                return m_cells[r][c];
            } else if (checkDirection(r,c,1,-1)) {
                m_winner = m_cells[r][c];
                return m_cells[r][c];
            }
        }
    }
    return EMPTY;
}

bool C4Board::isEmpty() const {
    return (std::count(&m_cells[0][0], &m_cells[0][0]+42, EMPTY) == 42);
}

int C4Board::getClickedColumn(crvec2 mouse) const {
    for (int i = 0; i < 7; i++) {
        if (mouse.x >= -75+i*150/7 && mouse.x <= -75+(i+1)*150/7 && mouse.y >= -75 && mouse.y <= 75) {
            return i;
        }
    }
    return -1;
}

glm::vec4 C4Board::getNumRows() const {
    auto checkDirection =
        [&](int r, int c, int dr, int dc) {
            for (int i = 1; i < 4; i++) {
                if (m_cells[r][c] != m_cells[r+i*dr][c+i*dc] || !inRange(r+i*dr,0,5) || !inRange(c+i*dc,0,6)) {
                    return i;
                }
            }
            return 4;
        };

     glm::vec4 pcounter(0), ccounter(0);
     for (int r = 0; r < 6; r++) {
        for (int c = 0; c < 7; c++) {
            if (m_cells[r][c] == PLAYER) {
                pcounter[checkDirection(r,c,0,1)-1]++;
                pcounter[checkDirection(r,c,1,1)-1]++;
                pcounter[checkDirection(r,c,1,0)-1]++;
                pcounter[checkDirection(r,c,1,-1)-1]++;
            } else if (m_cells[r][c] == COMPUTER) {
                ccounter[checkDirection(r,c,0,1)-1]++;
                ccounter[checkDirection(r,c,1,1)-1]++;
                ccounter[checkDirection(r,c,1,0)-1]++;
                ccounter[checkDirection(r,c,1,-1)-1]++;
            }
        }
    }
    return glm::vec4(ccounter[1],ccounter[2],pcounter[1],pcounter[2]);
}

void C4Board::switchTurn() {
    m_currPlayer = (m_currPlayer == PLAYER) ? COMPUTER : PLAYER;
}

void C4Board::reset() {
    m_currPlayer = nta::Random::randBool() ? PLAYER : COMPUTER;
    m_winner = EMPTY;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            m_cells[i][j] = EMPTY;
        }
    }
}

void C4Board::drawCircs(nta::SpriteBatch& sBatch, nta::SpriteFont* font) const {
    static int boarder = 1;
    static float width = 150/7-2*boarder;
    static float height = 25-2*boarder;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            glm::vec4 col(0);
            if (m_cells[i][j] == PLAYER) {
                col = PLAYER_COLOR;
            } else if (m_cells[i][j] == COMPUTER) {
                col = COMPUTER_COLOR;
            }
            font->drawText(sBatch, "O", glm::vec4(-75+boarder+j*150/7,75-boarder-25*i,width,height), col);
        }
    }

    std::string text = (m_currPlayer == PLAYER) ? "Player's Turn" : "Computer's Turn";
    glm::vec4 col = (m_currPlayer == PLAYER) ? PLAYER_COLOR : COMPUTER_COLOR;
    if (m_winner == PLAYER) {
        col = PLAYER_COLOR;
        text = "Player won";
    } else if (m_winner == COMPUTER) {
        col = COMPUTER_COLOR;
        text = "Computer won";
    }
    font->drawText(sBatch, text, glm::vec4(-80,98,160,21), col);
}

void C4Board::drawPrims(nta::PrimBatch& pBatch) const {
    static GLuint id = nta::ResourceManager::getTexture("solid.png").id;
    for (int i = 0; i < 5; i++) {
        pBatch.addPrimitive({{glm::vec2(-75+150./7*(i+1),75)},{glm::vec2(-75+150./7*(i+1),-75)}},id);
        pBatch.addPrimitive({{glm::vec2(-75,75-25*(i+1))},{glm::vec2(75,75-25*(i+1))}},id);
    }
    pBatch.addPrimitive({{glm::vec2(-75+150./7*6,75)},{glm::vec2(-75+150./7*6,-75)}},id);
    pBatch.addPrimitive({{glm::vec2(-75,75)},{glm::vec2(-75,-75)}},id);
    pBatch.addPrimitive({{glm::vec2(-75,-75)},{glm::vec2(75,-75)}},id);
    pBatch.addPrimitive({{glm::vec2(75,-75)},{glm::vec2(75,75)}},id);
    pBatch.addPrimitive({{glm::vec2(75,75)},{glm::vec2(-75,75)}},id);
}

MMNode* C4Board::generateMove(int move) {
    return makeMove(move);
}

float C4Board::evaluate() const {
    /**/
    static float weights[] = {3,8};
    switch(m_winner) {
        case PLAYER:    return -100;
        case COMPUTER:  return 100;
    }
    glm::vec4 rows = getNumRows();
    return (weights[0]*rows[0] + weights[1]*rows[1])-((weights[0]+1)*rows[2] + (weights[1]+1)*rows[3]);
    /**
    float val = 0;
    for (int r = 0; r < 6; r++) {
        for (int c = 0; c < 7; c++) {
            if (m_cells[r][c] == m_cells[r][6-c]) {
                val++;
            } else if (std::min<int>(m_cells[r][c],m_cells[r][6-c]) == (int)EMPTY){
                val--;
            } else {
                val -= 10;
            }
        }
    }
    return val;
    /**/
}

bool C4Board::same(MMNode* rhs) const {
    C4Board* board = (C4Board*)rhs;
    for (int r = 5; r >= 0; r--) {
        for (int c = 0; c < 7; c++) {
            if (m_cells[r][c] != board->m_cells[r][c]) {
                return false;
            }
        }
    }
    return true;
}

int C4Board::getNumMoves() const {
    return 7;
}
