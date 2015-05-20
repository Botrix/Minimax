#include "../MyEngine/MainEngine/Logger.h"
#include "../MyEngine/MainEngine/ResourceManager.h"
#include "../MyEngine/MainEngine/InputManager.h"
#include "../MyEngine/MainEngine/SystemManager.h"
#include "../MyEngine/MainEngine/GLMConsoleOutput.h"

#include "Connect4.h"

Connect4::Connect4() {
}

Connect4::~Connect4() {
    delete m_cBoard;
}

void Connect4::init() {
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
    m_cBoard = new C4Board;
    m_cBoard->reset();
    nta::Logger::writeToLog("Initialized game objects");

    nta::Logger::writeToLog("game initialized");
}

void Connect4::onFocus() {
    m_state = nta::ScreenState::RUNNING;
    m_cBoard->reset();
}

void Connect4::update() {
    glm::vec2 mouseCoords = m_camera.mouseToGame(nta::InputManager::getMouseCoords(), m_window->getDimensions());
    if (nta::InputManager::justPressed(SDL_BUTTON_LEFT)) {
        m_cBoard = m_cBoard->makeMove(m_cBoard->getClickedColumn(mouseCoords));
    } else if (nta::InputManager::justPressed(SDLK_r)) {
        m_cBoard->reset();
    } else if (m_cBoard->getTurn() == COMPUTER) {
        Sleep(500);
        if (m_cBoard->isEmpty()) {
            m_cBoard = m_cBoard->makeMove(rand()%7);
        } else {
            m_tree = new MMTree(m_cBoard, 10);
            m_cBoard = new C4Board(*(C4Board*)m_tree->bestMove());
            delete m_tree;
            m_tree = nullptr;
        }
    }
}

void Connect4::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_batch.begin();
    m_cBoard->drawCircs(m_batch, m_font);
    m_batch.end();

    m_primBatch.begin();
    m_cBoard->drawPrims(m_primBatch);
    m_primBatch.end();

    ///simpleProg
    m_simpleProg->use();

    glUniformMatrix3fv(m_simpleProg->getUniformLocation("camera"), 1, GL_FALSE, &m_camera.getCameraMatrix()[0][0]);
    m_batch.render();
    m_primBatch.render();

    m_simpleProg->unuse();
    m_window->swapBuffers();
}
