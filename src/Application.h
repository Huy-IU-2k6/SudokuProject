#ifndef APPLICATION_H
#define APPLICATION_H

#include <SFML/Graphics.hpp>
#include "core/board.h"
#include "gui/SudokuGrid.h"
#include "gui/GameUI.h"
#include "gui/DifficultyBar.h"
#include "gui/MainMenu.h"

class Application {
public:
    Application();
    void run();

private:
    void processEvents();
    void render();
    enum class State {
        MainMenu,
        Playing
    };
    
    sf::RenderWindow mWindow;
    sf::Font mFont;         // Nạp font chữ 1 lần ở đây
    Board mBoard;           // Bảng chứa dữ liệu thật (Logic)
    SudokuGrid mGridView; 
    GUI::GameUI mGameUI;  // Chịu trách nhiệm vẽ bảng (GUI)
    GUI::DifficultyBar mDifficultyBar; // <-- DÒNG MỚI NÀY
    GUI::MainMenu mMainMenu; // <-- DÒNG MỚI NÀY
    State mCurrentState;
};

#endif