#ifndef DIFFICULTY_MENU_H
#define DIFFICULTY_MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include "Button.h"

namespace GUI {

class DifficultyMenu : public sf::Drawable {
public:
    using Callback = std::function<void(const std::string&)>;

    DifficultyMenu(const sf::Font& font);

    void buildUI(sf::Vector2f screenSize);
    void update(const sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    
    void setDifficultyCallback(Callback callback);
    
    // ĐÂY CHÍNH LÀ 2 HÀM BỊ THIẾU ĐANG BÁO LỖI:
    void setCancelText(const std::string& text);
    void setCancelCallback(std::function<void()> callback);

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    const sf::Font& mFont;
    sf::Text mTitleText;
    std::vector<std::unique_ptr<Button>> mButtons;
    std::unique_ptr<Button> mCancelButton; // Nút Cancel/Return
    
    Callback mDifficultyCallback;
    std::function<void()> mCancelCallback;

    static constexpr unsigned int TITLE_FONT_SIZE = 60;
    static constexpr float BTN_WIDTH = 220.f;
    static constexpr float BTN_HEIGHT = 55.f;
    static constexpr float SPACING_X = 40.f;
    static constexpr float SPACING_Y = 25.f;
    static constexpr float START_Y_PERCENT = 0.38f;

    static constexpr float CANCEL_BTN_WIDTH = 260.f;
    static constexpr float CANCEL_BTN_HEIGHT = 50.f;

    static const sf::Color COLOR_TITLE;
    static const sf::Color COLOR_BTN_NORMAL;
    static const sf::Color COLOR_BTN_HOVER;
    static const sf::Color COLOR_TEXT;
    static const sf::Color COLOR_CANCEL_NORMAL;
    static const sf::Color COLOR_CANCEL_HOVER;
};

}

#endif