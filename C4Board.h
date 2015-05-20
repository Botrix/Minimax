#ifndef C4BOARD_H_INCLUDED
#define C4BOARD_H_INCLUDED

#include "../MyEngine/MainEngine/PrimitiveBatch.h"
#include "../MyEngine/MainEngine/SpriteFont.h"

#include "TTTBoard.h"

#define PLAYER_COLOR        glm::vec4(1,0,0,1)
#define COMPUTER_COLOR      glm::vec4(0,0,1,1)

class C4Board : public MMNode {
private:
    void                    switchTurn();

    Cell                    m_cells[6][7];
    Cell                    m_currPlayer;
    Cell                    m_winner;
public:
                            C4Board();
                            ~C4Board();
    C4Board*                makeMove(int col) const;
    Cell                    getTurn() const;
    Cell                    checkWinner();
    glm::vec4               getNumRows() const;
    bool                    isEmpty() const;
    int                     getClickedColumn(crvec2 mouse) const;
    void                    reset();
    void                    drawPrims(nta::PrimitiveBatch& pBatch) const;
    void                    drawCircs(nta::SpriteBatch& sBatch, nta::SpriteFont* font) const;

    MMNode*                 generateMove(int move);
    float                   evaluate() const;
    bool                    same(MMNode* rhs) const;
    int                     getNumMoves() const;
};

#endif // C4BOARD_H_INCLUDED
