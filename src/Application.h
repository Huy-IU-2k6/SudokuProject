#ifndef APPLICATION_H
#define APPLICATION_H

#include <SFML/Graphics.hpp>
#include "core/board.h"
#include "gui/StateStack.h"
#include <string>

class Application {
public:
    Application();
    void run();

private:
    void processEvents();
    void render();
    void registerStates();

private:
    sf::RenderWindow mWindow;
    sf::Font mFont;         
    Board mBoard;    
    std::string mCurrentDifficulty;       
    StateStack mStateStack; // The ultimate state manager
};

#endif