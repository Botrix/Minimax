#ifndef TICTACTOE_H_INCLUDED
#define TICTACTOE_H_INCLUDED

#include "../MyEngine/MainEngine/Window.h"
#include "../MyEngine/MainEngine/FPSLimiter.h"
#include "../MyEngine/MainEngine/GLSLProgram.h"
#include "../MyEngine/MainEngine/SpriteFont.h"
#include "../MyEngine/MainEngine/PrimitiveBatch.h"
#include "../MyEngine/MainEngine/Camera2D.h"
#include "../MyEngine/MainEngine/Screen.h"

#include "TTTBoard.h"

class TicTacToe : public nta::Screen {
private:
    nta::GLSLProgram*       m_simpleProg = nullptr;
    nta::SpriteFont*        m_font = nullptr;
    nta::SpriteBatch        m_batch;
    nta::PrimBatch          m_primBatch;
    nta::Camera2D           m_camera;

    TTTBoard*               m_tBoard = nullptr;
    MMTree*                 m_tree = nullptr;
public:
                            TicTacToe();
                            ~TicTacToe();
    void                    init();
    void                    update();
    void                    render();
    void                    onFocus();
};

#endif // TICTACTOE_H_INCLUDED
