#ifndef TTTBOARD_H_INCLUDED
#define TTTBOARD_H_INCLUDED

#include "../MyEngine/MainEngine/SpriteFont.h"
#include "../MyEngine/MainEngine/PrimitiveBatch.h"
#include "../MyEngine/Search/MMTree.h"

using namespace nta::Search;

enum Cell{EMPTY=32, PLAYER=88, COMPUTER=79};

class TTTBoard : public MMNode {
private:
    void                    switchTurn();

    Cell                    m_cells[9];
    Cell                    m_currPlayer;
    Cell                    m_winner;
    std::vector<int>        m_winningCells;
public:
                            TTTBoard();
                            ~TTTBoard();
    TTTBoard*               makeMove(int cell);
    Cell                    checkVictory();
    Cell                    getTurn() const;
    bool                    isEmpty() const;
    int                     getNumWaysToWin() const;
    int                     getClickedCell(crvec2 mouse) const;
    void                    reset();
    void                    draw(nta::PrimBatch& pbatch) const;
    void                    drawText(nta::SpriteBatch& sbatch, nta::SpriteFont* font) const;

    MMNode*                 generateMove(int move);
    float                   evaluate() const;
    bool                    same(MMNode* rhs) const;
    int                     getNumMoves() const;
};

#endif // TTTBOARD_H_INCLUDED
