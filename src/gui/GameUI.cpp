#include "GameUI.h"
#include <iostream>

namespace GUI {

GameUI::GameUI(const sf::Font& font) 
    : mFont(font)
    , mMistakesText(font) 
    , mTimeText(font) 
    , mNewGameButton(nullptr)
{
}

void GameUI::buildUI() {
    initTexts();
    initActionButtons();
    initNumpad();
    initNewGameButton();
}

void GameUI::initTexts() {
    mMistakesText.setCharacterSize(26);
    mMistakesText.setFillColor(sf::Color(80, 80, 80));
    mMistakesText.setPosition({PANEL_START_X, PANEL_START_Y});
    setMistakes(0, 3); 

    mTimeText.setCharacterSize(26);
    mTimeText.setFillColor(sf::Color(80, 80, 80));
    mTimeText.setPosition({PANEL_START_X, PANEL_START_Y + 45.f});
    setTime("00:00");
}

void GameUI::initActionButtons() {
    float startY = PANEL_START_Y + 110.f;
    std::vector<std::string> actionNames = {"Undo", "Erase", "Hint", "Solution"};

    // 2x2 grid layout algorithm for the 4 action buttons
    for (size_t i = 0; i < actionNames.size(); ++i) {
        int row = i / 2; // Row 0 or 1
        int col = i % 2; // Column 0 or 1

        auto btn = std::make_unique<Button>(mFont);
        btn->setButtonSize({ACT_BTN_WIDTH, ACT_BTN_HEIGHT});
        btn->setText(actionNames[i]);

        // Calculate coordinates based on row and column positions
        float posX = PANEL_START_X + col * (ACT_BTN_WIDTH + ACT_SPACING_X);
        float posY = startY + row * (ACT_BTN_HEIGHT + ACT_SPACING_Y);
        btn->setPosition({posX, posY});
        
        std::string name = actionNames[i];
        btn->setCallback([name]() {
            std::cout << "Action activated: " << name << "\n";
        });

        mActionButtons.push_back(std::move(btn));
    }
}

void GameUI::initNumpad() {
    for (int i = 0; i < 9; ++i) {
        int row = i / 3;
        int col = i % 3;

        auto btn = std::make_unique<Button>(mFont);
        btn->setButtonSize({NUMPAD_SIZE, NUMPAD_SIZE});
        btn->setText(std::to_string(i + 1));
        
        float posX = PANEL_START_X + col * (NUMPAD_SIZE + NUMPAD_SPACING);
        float posY = NUMPAD_START_Y + row * (NUMPAD_SIZE + NUMPAD_SPACING);
        btn->setPosition({posX, posY});

        int number = i + 1;
        btn->setCallback([number]() {
            std::cout << "Number selected for cell: " << number << "\n";
        });

        mNumpadButtons.push_back(std::move(btn));
    }
}

void GameUI::initNewGameButton() {
    mNewGameButton = std::make_unique<Button>(mFont);
    mNewGameButton->setButtonSize({NEW_GAME_WIDTH, NEW_GAME_HEIGHT});
    mNewGameButton->setText("New Game");
    mNewGameButton->setPosition({PANEL_START_X, NEW_GAME_START_Y});
    
    mNewGameButton->setCallback([]() {
        std::cout << "Starting new game!\n";
    });
}

void GameUI::setMistakes(int mistakes, int maxMistakes) {
    mMistakesText.setString("Mistakes: " + std::to_string(mistakes) + "/" + std::to_string(maxMistakes));
}

void GameUI::setTime(const std::string& timeStr) {
    mTimeText.setString("Time: " + timeStr);
}

void GameUI::update(const sf::RenderWindow& window) {
    for (auto& btn : mActionButtons) btn->update(window);
    for (auto& btn : mNumpadButtons) btn->update(window);
    if (mNewGameButton) mNewGameButton->update(window);
}

void GameUI::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    for (auto& btn : mActionButtons) btn->handleEvent(event, window);
    for (auto& btn : mNumpadButtons) btn->handleEvent(event, window);
    if (mNewGameButton) mNewGameButton->handleEvent(event, window);
}

void GameUI::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(mMistakesText, states);
    target.draw(mTimeText, states);
    
    for (const auto& btn : mActionButtons) target.draw(*btn, states);
    for (const auto& btn : mNumpadButtons) target.draw(*btn, states);
    if (mNewGameButton) target.draw(*mNewGameButton, states);
}

}