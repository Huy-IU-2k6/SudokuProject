#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

namespace GUI {

class Button : public sf::Drawable, public sf::Transformable {
public:
    using Callback = std::function<void()>;

    Button(const sf::Font& font);

    void setText(const std::string& text);
    void setCallback(Callback callback);
    void setButtonSize(sf::Vector2f size);
    
    // Upgraded: Set custom theme colors dynamically for individual buttons
    void setCustomColors(sf::Color normalBg, sf::Color hoverBg, sf::Color textColors);
    
    void update(const sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void centerText();

private:
    sf::RectangleShape mBackground;
    sf::Text mText;
    Callback mCallback;
    bool mIsHovered;

    // Dynamic color properties instead of hardcoded static values
    sf::Color mNormalBgColor;
    sf::Color mHoverBgColor;
    sf::Color mTextColor;

    static const sf::Color COLOR_DEFAULT_NORMAL;
    static const sf::Color COLOR_DEFAULT_HOVER;
    static const sf::Color COLOR_DEFAULT_TEXT;
    static const sf::Color COLOR_DEFAULT_OUTLINE;
    
    static constexpr float OUTLINE_THICKNESS = 2.f;
    static constexpr unsigned int DEFAULT_FONT_SIZE = 24;
};

}

#endif