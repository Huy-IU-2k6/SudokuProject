#include "Application.h"
#include <stdexcept>

// Include concrete states here
#include "gui/MainMenuState.h"
#include "gui/DifficultySelectionState.h"
#include "gui/PlayingState.h"

Application::Application()
    : mWindow(sf::VideoMode({1000, 700}), "Sudoku Platform", sf::Style::Default)
    , mFont()
    , mBoard()
    , mCurrentDifficulty("Easy") // Khởi tạo mặc định là Easy
    // Truyền thêm mCurrentDifficulty vào Context
    , mStateStack(State::Context(mWindow, mFont, mBoard, mCurrentDifficulty))
{
    mWindow.setFramerateLimit(60);
    
    if (!mFont.openFromFile("assets/fonts/arial.ttf")) {
        throw std::runtime_error("Critical error: Could not open font file arial.ttf!");
    }

    registerStates();
    
    // Push the initial bootstrap state into the stack
    mStateStack.pushState("MainMenu");
}

void Application::registerStates() {
    mStateStack.registerState<MainMenuState>("MainMenu");
    mStateStack.registerState<DifficultySelectionState>("DifficultySelection");
    mStateStack.registerState<PlayingState>("Playing");
}

void Application::run() {
    while (mWindow.isOpen()) {
        processEvents();
        
        mStateStack.update(mWindow);
        
        // Safety terminal check
        if (mStateStack.isEmpty()) {
            mWindow.close();
        }
        
        render();
    }
}

void Application::processEvents() {
    while (const std::optional event = mWindow.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            mWindow.close();
        }
        
        if (const auto* resized = event->getIf<sf::Event::Resized>()) {
            float windowRatio = static_cast<float>(resized->size.x) / static_cast<float>(resized->size.y);
            float viewRatio = 1000.f / 700.f;
            float sizeX = 1.f, sizeY = 1.f, posX = 0.f, posY = 0.f;

            if (windowRatio > viewRatio) {
                sizeX = viewRatio / windowRatio;
                posX = (1.f - sizeX) / 2.f;
            } else {
                sizeY = windowRatio / viewRatio;
                posY = (1.f - sizeY) / 2.f;
            }

            sf::View view(sf::FloatRect({0.f, 0.f}, {1000.f, 700.f}));
            view.setViewport(sf::FloatRect({posX, posY}, {sizeX, sizeY}));
            mWindow.setView(view);
        }

        mStateStack.handleEvent(*event, mWindow);
    }
}

void Application::render() {
    mWindow.clear(sf::Color(250, 250, 250)); 

    sf::View currentView = mWindow.getView();
    mWindow.setView(mWindow.getDefaultView());
    sf::RectangleShape background(sf::Vector2f(mWindow.getSize()));
    background.setFillColor(sf::Color(250, 250, 250));
    mWindow.draw(background);
    mWindow.setView(currentView);

    // Let the stack draw all visible layers sequentially
    mStateStack.draw();

    mWindow.display();
}