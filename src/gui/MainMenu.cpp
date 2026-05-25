#include "MainMenu.h"

namespace GUI {

const sf::Color MainMenu::COLOR_TITLE_BLUE       = sf::Color(65, 105, 225); // Royal Blue
const sf::Color MainMenu::COLOR_START_GREEN      = sf::Color(46, 204, 113); // Emerald Green
const sf::Color MainMenu::COLOR_START_GREEN_HOVER= sf::Color(39, 174, 96);  // Darker Green
const sf::Color MainMenu::COLOR_EXIT_RED         = sf::Color(231, 76, 60);  // Alizarin Red
const sf::Color MainMenu::COLOR_EXIT_RED_HOVER   = sf::Color(192, 57, 43);  // Darker Red
const sf::Color MainMenu::COLOR_TEXT_WHITE       = sf::Color::White;

MainMenu::MainMenu(const sf::Font& font)
    : mFont(font)
    , mTitleText(font)
    , mStartButton(nullptr)
    , mExitButton(nullptr)
{
}

void MainMenu::buildUI(sf::Vector2f screenKeySize) {
    // 1. Configure the "Sudoku Game" title centered horizontally
    mTitleText.setString("Sudoku Game");
    mTitleText.setCharacterSize(TITLE_FONT_SIZE);
    mTitleText.setFillColor(COLOR_TITLE_BLUE);
    mTitleText.setStyle(sf::Text::Bold);

    sf::FloatRect titleBounds = mTitleText.getLocalBounds();
    mTitleText.setOrigin({
        titleBounds.position.x + (titleBounds.size.x / 2.f),
        titleBounds.position.y + (titleBounds.size.y / 2.f)
    });
    // Position at 25% height of the screen
    mTitleText.setPosition({screenKeySize.x / 2.f, screenKeySize.y * 0.25f});

    // 2. Configure the green Start Button centered horizontally
    mStartButton = std::make_unique<Button>(mFont);
    mStartButton->setButtonSize({BTN_WIDTH, BTN_HEIGHT});
    mStartButton->setText("Start");
    mStartButton->setCustomColors(COLOR_START_GREEN, COLOR_START_GREEN_HOVER, COLOR_TEXT_WHITE);
    // Position at 50% height of the screen
    mStartButton->setPosition({screenKeySize.x / 2.f - BTN_WIDTH / 2.f, screenKeySize.y * 0.50f});

    // 3. Configure the red Exit Button directly below the Start Button
    mExitButton = std::make_unique<Button>(mFont);
    mExitButton->setButtonSize({BTN_WIDTH, BTN_HEIGHT});
    mExitButton->setText("Exit");
    mExitButton->setCustomColors(COLOR_EXIT_RED, COLOR_EXIT_RED_HOVER, COLOR_TEXT_WHITE);
    // Position right after Start button + spacing
    float exitYPos = screenKeySize.y * 0.50f + BTN_HEIGHT + COMPONENT_SPACING_Y;
    mExitButton->setPosition({screenKeySize.x / 2.f - BTN_WIDTH / 2.f, exitYPos});
}

void MainMenu::setStartCallback(std::function<void()> callback) {
    if (mStartButton) mStartButton->setCallback(callback);
}

void MainMenu::setExitCallback(std::function<void()> callback) {
    if (mExitButton) mExitButton->setCallback(callback);
}

void MainMenu::update(const sf::RenderWindow& window) {
    if (mStartButton) mStartButton->update(window);
    if (mExitButton) mExitButton->update(window);
}

void MainMenu::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (mStartButton) mStartButton->handleEvent(event, window);
    if (mExitButton) mExitButton->handleEvent(event, window);
}

void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(mTitleText, states);
    if (mStartButton) target.draw(*mStartButton, states);
    if (mExitButton) target.draw(*mExitButton, states);
}

}