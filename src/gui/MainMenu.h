#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <functional>
#include "Button.h"

namespace GUI {

class MainMenu : public sf::Drawable {
public:
    MainMenu(const sf::Font& font);

    void buildUI(sf::Vector2f screenKeySize);
    void update(const sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);

    void setStartCallback(std::function<void()> callback);
    void setExitCallback(std::function<void()> callback);

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    const sf::Font& mFont;
    sf::Text mTitleText;
    std::unique_ptr<Button> mStartButton;
    std::unique_ptr<Button> mExitButton;

    // Layout configuration constants to avoid magic numbers
    static constexpr unsigned int TITLE_FONT_SIZE = 72;
    static constexpr float BTN_WIDTH = 260.f;
    static constexpr float BTN_HEIGHT = 60.f;
    static constexpr float COMPONENT_SPACING_Y = 40.f;

    // Exact color specifications from user requirements
    static const sf::Color COLOR_TITLE_BLUE;
    static const sf::Color COLOR_START_GREEN;
    static const sf::Color COLOR_START_GREEN_HOVER;
    static const sf::Color COLOR_EXIT_RED;
    static const sf::Color COLOR_EXIT_RED_HOVER;
    static const sf::Color COLOR_TEXT_WHITE;
};

}

#endif