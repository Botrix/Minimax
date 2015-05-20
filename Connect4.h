#ifndef CONNECT4_H_INCLUDED
#define CONNECT4_H_INCLUDED

#include "../MyEngine/MainEngine/Window.h"
#include "../MyEngine/MainEngine/FPSLimiter.h"
#include "../MyEngine/MainEngine/GLSLProgram.h"
#include "../MyEngine/MainEngine/SpriteFont.h"
#include "../MyEngine/MainEngine/PrimitiveBatch.h"
#include "../MyEngine/MainEngine/Camera2D.h"
#include "../MyEngine/MainEngine/Screen.h"

#include "C4Board.h"

class Connect4 : public nta::Screen {
private:
    nta::GLSLProgram*       m_simpleProg = nullptr;
    nta::SpriteFont*        m_font = nullptr;
    nta::SpriteBatch        m_batch;
    nta::PrimBatch          m_primBatch;
    nta::Camera2D           m_camera;

    C4Board*                m_cBoard = nullptr;
    MMTree*                 m_tree = nullptr;
public:
                            Connect4();
                            ~Connect4();
    void                    init();
    void                    update();
    void                    render();
    void                    onFocus();
};

#endif // CONNECT4_H_INCLUDED
