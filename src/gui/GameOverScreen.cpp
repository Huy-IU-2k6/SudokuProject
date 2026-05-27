#include "GameOverScreen.h"

namespace GUI {

// SỬA LỖI 1: Truyền font vào Text ngay từ Danh sách khởi tạo
GameOverScreen::GameOverScreen(const sf::Font& font) 
    : mTitle(font)
    , mMessage(font) 
{
    // 1. Nền mờ
    mOverlay.setSize({1000.f, 700.f});
    mOverlay.setFillColor(sf::Color(255, 255, 255, 200));

    // 2. Khung thoại
    mPopupBox.setSize({500.f, 250.f});
    mPopupBox.setFillColor(sf::Color::White);
    mPopupBox.setOutlineColor(sf::Color(200, 200, 200));
    mPopupBox.setOutlineThickness(2.f);
    mPopupBox.setOrigin({250.f, 125.f});
    mPopupBox.setPosition({500.f, 350.f});

    // 3. Tiêu đề
    mTitle.setCharacterSize(34);
    mTitle.setStyle(sf::Text::Bold);
    mTitle.setString("Game Over");
    mTitle.setFillColor(sf::Color(44, 62, 80));
    centerTextOrigin(mTitle);
    mTitle.setPosition({500.f, 265.f});

    // 4. Lời nhắn
    mMessage.setCharacterSize(22);
    mMessage.setFillColor(sf::Color(100, 100, 100));
    mMessage.setString("You have made 3 mistakes and lost\nthis game");
    centerTextOrigin(mMessage);
    mMessage.setPosition({500.f, 320.f});

    // 5. Nút Second Chance
    mSecondChanceBtn = std::make_unique<Button>(font);
    mSecondChanceBtn->setButtonSize({300.f, 50.f});
    mSecondChanceBtn->setText("Second Chance");
    mSecondChanceBtn->setPosition({350.f, 360.f});
    mSecondChanceBtn->setCallback([this]() {
        if (mSecondChanceCallback) mSecondChanceCallback();
    });

    // 6. Nút New Game
    mNewGameBtn = std::make_unique<Button>(font);
    mNewGameBtn->setButtonSize({300.f, 50.f});
    mNewGameBtn->setText("New Game");
    mNewGameBtn->setPosition({350.f, 420.f});
    mNewGameBtn->setCallback([this]() {
        if (mNewGameCallback) mNewGameCallback();
    });
}

void GameOverScreen::centerTextOrigin(sf::Text& text) {
    sf::FloatRect bounds = text.getLocalBounds();
    // SỬA LỖI 2: Dùng cú pháp của SFML 3 (position và size)
    text.setOrigin({bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f});
}

void GameOverScreen::setSecondChanceCallback(std::function<void()> callback) { mSecondChanceCallback = std::move(callback); }
void GameOverScreen::setNewGameCallback(std::function<void()> callback) { mNewGameCallback = std::move(callback); }

void GameOverScreen::update(const sf::RenderWindow& window) {
    mSecondChanceBtn->update(window);
    mNewGameBtn->update(window);
}

void GameOverScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    mSecondChanceBtn->handleEvent(event, window);
    mNewGameBtn->handleEvent(event, window);
}

void GameOverScreen::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(mOverlay, states);
    target.draw(mPopupBox, states);
    target.draw(mTitle, states);
    target.draw(mMessage, states);
    target.draw(*mSecondChanceBtn, states);
    target.draw(*mNewGameBtn, states);
}

}