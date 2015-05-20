#include <SDL2/SDL_ttf.h>

#include "../MyEngine/MainEngine/ResourceManager.h"
#include "../MyEngine/MainEngine/SystemManager.h"
#include "../MyEngine/MainEngine/AudioManager.h"
#include "../MyEngine/MainEngine/ScreenManager.h"
#include "../MyEngine/MainEngine/Logger.h"
#include "../MyEngine/MainEngine/Random.h"

#include "TicTacToe.h"
#include "Connect4.h"
#include "Menu.h"

void initGL() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc, char* argv[]) {
    nta::init();
    nta::Logger::createLog();
    nta::Random::init();
    nta::AudioManager::init();

    nta::ScreenManager screenManager("Minimax", 60);
    screenManager.addScreen(new Menu);
    screenManager.addScreen(new TicTacToe, 0);
    screenManager.addScreen(new Connect4, 0);

    initGL();
    screenManager.run();

    screenManager.destroy();
    nta::ResourceManager::destroy();
    nta::SystemManager::destroy();
    nta::AudioManager::destroy();
    TTF_Quit();
    SDL_Quit();
    nta::Logger::writeToLog("Program exited cleanly");

    return 0;
}
