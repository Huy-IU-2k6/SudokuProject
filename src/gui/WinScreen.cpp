#include "WinScreen.h"

namespace GUI {

// SỬA LỖI 1: Truyền font vào Text ngay từ Danh sách khởi tạo
WinScreen::WinScreen(const sf::Font& font) 
    : mTitle(font)
    , mMessage(font)
{
    mOverlay.setSize({1000.f, 700.f});
    mOverlay.setFillColor(sf::Color(255, 255, 255, 200));

    mPopupBox.setSize({500.f, 250.f});
    mPopupBox.setFillColor(sf::Color::White);
    mPopupBox.setOutlineColor(sf::Color(200, 200, 200));
    mPopupBox.setOutlineThickness(2.f);
    mPopupBox.setOrigin({250.f, 125.f});
    mPopupBox.setPosition({500.f, 350.f});

    mTitle.setCharacterSize(34);
    mTitle.setStyle(sf::Text::Bold);
    mTitle.setString("Congratulations!");
    mTitle.setFillColor(sf::Color(39, 174, 96)); // Màu xanh lá
    centerTextOrigin(mTitle);
    mTitle.setPosition({500.f, 265.f});

    mMessage.setCharacterSize(22);
    mMessage.setFillColor(sf::Color(100, 100, 100));
    mMessage.setString("You have successfully solved\nthe puzzle!");
    centerTextOrigin(mMessage);
    mMessage.setPosition({500.f, 320.f});

    mNewGameBtn = std::make_unique<Button>(font);
    mNewGameBtn->setButtonSize({300.f, 50.f});
    mNewGameBtn->setText("New Game");
    mNewGameBtn->setPosition({350.f, 390.f}); // Đẩy nút vào giữa
    mNewGameBtn->setCallback([this]() {
        if (mNewGameCallback) mNewGameCallback();
    });
}

void WinScreen::centerTextOrigin(sf::Text& text) {
    sf::FloatRect bounds = text.getLocalBounds();
    // SỬA LỖI 2: Dùng cú pháp của SFML 3 (position và size)
    text.setOrigin({bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f});
}

void WinScreen::setNewGameCallback(std::function<void()> callback) { mNewGameCallback = std::move(callback); }

void WinScreen::update(const sf::RenderWindow& window) {
    mNewGameBtn->update(window);
}

void WinScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    mNewGameBtn->handleEvent(event, window);
}

void WinScreen::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(mOverlay, states);
    target.draw(mPopupBox, states);
    target.draw(mTitle, states);
    target.draw(mMessage, states);
    target.draw(*mNewGameBtn, states);
}

}