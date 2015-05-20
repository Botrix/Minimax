#include "../MyEngine/MainEngine/Logger.h"
#include "../MyEngine/MainEngine/InputManager.h"
#include "../MyEngine/MainEngine/ResourceManager.h"
#include "../MyEngine/MainEngine/SystemManager.h"

#include "Menu.h"

Menu::Menu() {
}

Menu::~Menu() {
}

void Menu::init() {
    nta::Logger::writeToLog("Initializing menu...");

    m_state = nta::ScreenState::RUNNING;
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

    m_hudBatch.init();
    m_font = nta::ResourceManager::getSpriteFont("chintzy.ttf", 64);

    m_buttons[0].bounds = glm::vec4(-60, 20, 120, 20);
    m_buttons[0].backgroundColor = glm::vec4(0);
    m_buttons[0].name = "Tic-Tac-Toe";

    m_buttons[1].bounds = glm::vec4(-60, -10, 120, 20);
    m_buttons[1].backgroundColor = glm::vec4(0);
    m_buttons[1].name = "Connect 4";
    nta::Logger::writeToLog("Initialized menu");
}

void Menu::update() {
    glm::vec2 mouseCoords = m_hudCamera.mouseToGame(nta::InputManager::getMouseCoords(), m_window->getDimensions());
    for (int i = 0; i < sizeof(m_buttons)/sizeof(Button); i++) {
        if (m_buttons[i].isPressed(mouseCoords) && nta::InputManager::justPressed(SDL_BUTTON_LEFT)) {
            m_nextIndex = i+1;
            m_state = nta::ScreenState::SWITCH;
        }
    }
}

void Menu::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static nta::GLTexture backgroundImage = nta::ResourceManager::getTexture("background.png");
    m_hudBatch.begin();
    m_hudBatch.addGlyph(glm::vec4(-100,100,200,200), glm::vec4(0,0,1,1), backgroundImage.id, 1, glm::vec4(1,1,1,.2));
    m_font->drawText(m_hudBatch, m_window->getTitle(), glm::vec4(-80, 90, 160, 20));
    for (int i = 0; i < sizeof(m_buttons)/sizeof(Button); i++)
        m_buttons[i].draw(m_font, m_hudBatch);
    m_hudBatch.end();

    ///simpleProg
    m_simpleProg->use();

    glUniformMatrix3fv(m_simpleProg->getUniformLocation("camera"), 1, GL_FALSE, &m_hudCamera.getCameraMatrix()[0][0]);
    m_hudBatch.render();

    m_simpleProg->unuse();
    m_window->swapBuffers();
}
