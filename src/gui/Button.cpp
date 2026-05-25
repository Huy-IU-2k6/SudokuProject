#include "Button.h"

namespace GUI {

const sf::Color Button::COLOR_DEFAULT_NORMAL  = sf::Color(245, 245, 245);
const sf::Color Button::COLOR_DEFAULT_HOVER   = sf::Color(220, 230, 242);
const sf::Color Button::COLOR_DEFAULT_TEXT    = sf::Color(40, 40, 40);
const sf::Color Button::COLOR_DEFAULT_OUTLINE = sf::Color(160, 160, 160);

Button::Button(const sf::Font& font)
    : mBackground()
    , mText(font)
    , mCallback(nullptr)
    , mIsHovered(false)
    , mNormalBgColor(COLOR_DEFAULT_NORMAL)
    , mHoverBgColor(COLOR_DEFAULT_HOVER)
    , mTextColor(COLOR_DEFAULT_TEXT)
{
    mBackground.setFillColor(mNormalBgColor);
    mBackground.setOutlineColor(COLOR_DEFAULT_OUTLINE);
    mBackground.setOutlineThickness(OUTLINE_THICKNESS);

    mText.setCharacterSize(DEFAULT_FONT_SIZE);
    mText.setFillColor(mTextColor);
}

void Button::setCustomColors(sf::Color normalBg, sf::Color hoverBg, sf::Color textColor) {
    mNormalBgColor = normalBg;
    mHoverBgColor = hoverBg;
    mTextColor = textColor;
    
    mBackground.setFillColor(mNormalBgColor);
    mText.setFillColor(mTextColor);
}

void Button::setText(const std::string& text) {
    mText.setString(text);
    centerText();
}

void Button::setButtonSize(sf::Vector2f size) {
    mBackground.setSize(size);
    centerText();
}

void Button::setCallback(Callback callback) {
    mCallback = std::move(callback);
}

void Button::update(const sf::RenderWindow& window) {
    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mouseCoords = window.mapPixelToCoords(mousePixelPos);

    sf::FloatRect globalBounds = getTransform().transformRect(mBackground.getLocalBounds());
    mIsHovered = globalBounds.contains(mouseCoords);

    if (mIsHovered) {
        mBackground.setFillColor(mHoverBgColor);
    } else {
        mBackground.setFillColor(mNormalBgColor);
    }
}

void Button::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (const auto* mouseClick = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseClick->button == sf::Mouse::Button::Left && mIsHovered) {
            if (mCallback) {
                mCallback();
            }
        }
    }
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(mBackground, states);
    target.draw(mText, states);
}

void Button::centerText() {
    sf::FloatRect textBounds = mText.getLocalBounds();
    sf::Vector2f bgSize = mBackground.getSize();

    mText.setOrigin({
        textBounds.position.x + (textBounds.size.x / 2.f),
        textBounds.position.y + (textBounds.size.y / 2.f)
    });

    mText.setPosition({
        bgSize.x / 2.f,
        bgSize.y / 2.f
    });
}

}