#include "DifficultyMenu.h"

namespace GUI {

const sf::Color DifficultyMenu::COLOR_TITLE         = sf::Color(65, 105, 225); 
const sf::Color DifficultyMenu::COLOR_BTN_NORMAL    = sf::Color(245, 245, 245); 
const sf::Color DifficultyMenu::COLOR_BTN_HOVER     = sf::Color(220, 230, 242); 
const sf::Color DifficultyMenu::COLOR_TEXT          = sf::Color(50, 50, 50);
const sf::Color DifficultyMenu::COLOR_CANCEL_NORMAL = sf::Color(180, 185, 190); 
const sf::Color DifficultyMenu::COLOR_CANCEL_HOVER  = sf::Color(140, 145, 150); 

DifficultyMenu::DifficultyMenu(const sf::Font& font)
    : mFont(font)
    , mTitleText(font)
    , mCancelButton(nullptr)
    , mDifficultyCallback(nullptr)
    , mCancelCallback(nullptr)
{
}

void DifficultyMenu::buildUI(sf::Vector2f screenSize) {
    mTitleText.setString("Select Difficulty");
    mTitleText.setCharacterSize(TITLE_FONT_SIZE);
    mTitleText.setFillColor(COLOR_TITLE);
    mTitleText.setStyle(sf::Text::Bold);

    sf::FloatRect titleBounds = mTitleText.getLocalBounds();
    mTitleText.setOrigin({
        titleBounds.position.x + (titleBounds.size.x / 2.f),
        titleBounds.position.y + (titleBounds.size.y / 2.f)
    });
    mTitleText.setPosition({screenSize.x / 2.f, screenSize.y * 0.18f});

    std::vector<std::string> levels = {"Easy", "Medium", "Hard", "Expert", "Master", "Extreme"};
    float startY = screenSize.y * START_Y_PERCENT;

    for (size_t i = 0; i < levels.size(); ++i) {
        int row = i / 2; 
        int col = i % 2; 

        auto btn = std::make_unique<Button>(mFont);
        btn->setButtonSize({BTN_WIDTH, BTN_HEIGHT});
        btn->setText(levels[i]);
        btn->setCustomColors(COLOR_BTN_NORMAL, COLOR_BTN_HOVER, COLOR_TEXT);

        float posX = (col == 0) ? (screenSize.x / 2.f - BTN_WIDTH - SPACING_X / 2.f)
                                : (screenSize.x / 2.f + SPACING_X / 2.f);
        float posY = startY + row * (BTN_HEIGHT + SPACING_Y);
        
        btn->setPosition({posX, posY});

        std::string levelName = levels[i];
        btn->setCallback([this, levelName]() {
            if (mDifficultyCallback) mDifficultyCallback(levelName);
        });

        mButtons.push_back(std::move(btn));
    }

    // Build the dynamic Cancel / Return Button centered at the bottom
    mCancelButton = std::make_unique<Button>(mFont);
    mCancelButton->setButtonSize({CANCEL_BTN_WIDTH, CANCEL_BTN_HEIGHT});
    mCancelButton->setText("Cancel"); 
    mCancelButton->setCustomColors(COLOR_CANCEL_NORMAL, COLOR_CANCEL_HOVER, sf::Color::White);
    
    float cancelX = screenSize.x / 2.f - CANCEL_BTN_WIDTH / 2.f;
    float cancelY = startY + 3 * (BTN_HEIGHT + SPACING_Y) + 10.f; 
    mCancelButton->setPosition({cancelX, cancelY});
    
    mCancelButton->setCallback([this]() {
        if (mCancelCallback) mCancelCallback();
    });
}

void DifficultyMenu::setCancelText(const std::string& text) {
    if (mCancelButton) mCancelButton->setText(text);
}

void DifficultyMenu::setCancelCallback(std::function<void()> callback) {
    mCancelCallback = std::move(callback);
}

void DifficultyMenu::setDifficultyCallback(Callback callback) {
    mDifficultyCallback = std::move(callback);
}

void DifficultyMenu::update(const sf::RenderWindow& window) {
    for (auto& btn : mButtons) btn->update(window);
    if (mCancelButton) mCancelButton->update(window);
}

void DifficultyMenu::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    for (auto& btn : mButtons) btn->handleEvent(event, window);
    if (mCancelButton) mCancelButton->handleEvent(event, window);
}

void DifficultyMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(mTitleText, states);
    for (const auto& btn : mButtons) target.draw(*btn, states);
    if (mCancelButton) target.draw(*mCancelButton, states);
}

}