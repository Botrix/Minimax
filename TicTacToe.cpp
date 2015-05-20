#include "../MyEngine/MainEngine/Logger.h"
#include "../MyEngine/MainEngine/ResourceManager.h"
#include "../MyEngine/MainEngine/InputManager.h"
#include "../MyEngine/MainEngine/SystemManager.h"
#include "../MyEngine/MainEngine/GLMConsoleOutput.h"

#include "TicTacToe.h"

TicTacToe::TicTacToe() {
}

TicTacToe::~TicTacToe() {
    delete m_tBoard;
}

void TicTacToe::init() {
    nta::Logger::writeToLog("Initializing game...");

    nta::Logger::writeToLog("Initializing GLSLPrograms...");
    m_simpleProg = nta::SystemManager::getGLSLProgram("Simple2D");
    if (!m_simpleProg->isLinked()) {
        m_simpleProg->addAttribute("pos");
        m_simpleProg->addAttribute("color");
        m_simpleProg->addAttribute("uv");
        m_simpleProg->linkShaders();
    }
    m_simpleProg->use();
    glUniform1i(m_simpleProg->getUniformLocation("sampler"), 0);
    m_simpleProg->unuse();
    nta::Logger::writeToLog("Initialized GLSLPrograms");

    m_batch.init();
    m_primBatch.init();
    m_font = nta::ResourceManager::getSpriteFont("ManilaSansReg.otf", 64);

    nta::Logger::writeToLog("Initializing game objects...");
    m_tBoard = new TTTBoard;
    m_tBoard->reset();
    nta::Logger::writeToLog("Initialized game objects");

    nta::Logger::writeToLog("game initialized");
}

void TicTacToe::onFocus() {
    m_state = nta::ScreenState::RUNNING;
    m_tBoard->reset();
}

void TicTacToe::update() {
    glm::vec2 mouseCoords = m_camera.mouseToGame(nta::InputManager::getMouseCoords(), m_window->getDimensions());
    if (nta::InputManager::justPressed(SDL_BUTTON_LEFT)) {
        m_tBoard = m_tBoard->makeMove(m_tBoard->getClickedCell(mouseCoords));
    } else if (nta::InputManager::justPressed(SDLK_r)) {
        m_tBoard->reset();
    } else if (m_tBoard->getTurn() == COMPUTER) {
        Sleep(500);
        if (m_tBoard->isEmpty()) {
            m_tBoard = m_tBoard->makeMove(rand()%9);
        } else {
            m_tree = new MMTree(m_tBoard, 6);
            m_tBoard = new TTTBoard(*(TTTBoard*)m_tree->bestMove());
            delete m_tree;
            m_tree = nullptr;
        }
    }
}

void TicTacToe::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_batch.begin();
    m_tBoard->drawText(m_batch, m_font);
    m_batch.end();

    m_primBatch.begin();
    m_tBoard->draw(m_primBatch);
    m_primBatch.end();

    ///simpleProg
    m_simpleProg->use();

    glUniformMatrix3fv(m_simpleProg->getUniformLocation("camera"), 1, GL_FALSE, &m_camera.getCameraMatrix()[0][0]);
    m_batch.render();
    m_primBatch.render();

    m_simpleProg->unuse();
    m_window->swapBuffers();
}
