#ifndef GAME_UI_H
#define GAME_UI_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <functional>
#include "Button.h"

namespace GUI {

class GameUI : public sf::Drawable {
public:
    GameUI(const sf::Font& font);
    void buildUI();
    
    void update(const sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);

    void setMistakes(int mistakes, int maxMistakes);
    void setTime(const std::string& timeStr);
    
    // HÀM QUAN TRỌNG: Dùng để kết nối với StateStack
    void setNewGameCallback(std::function<void()> callback);

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
    void initTexts();
    void initActionButtons();
    void initNumpad();
    void initNewGameButton();

private:
    const sf::Font& mFont;

    sf::Text mMistakesText;
    sf::Text mTimeText;

    std::vector<std::unique_ptr<Button>> mActionButtons;
    std::vector<std::unique_ptr<Button>> mNumpadButtons;
    std::unique_ptr<Button> mNewGameButton;
    
    // Biến lưu trữ hành động chuyển màn hình
    std::function<void()> mNewGameCallback;

    // --- BỐ CỤC CHUẨN UX (Giống y hệt hình chụp của bạn) ---
    static constexpr float PANEL_START_X = 660.f;  
    static constexpr float PANEL_START_Y = 40.f;

    static constexpr float ACT_BTN_WIDTH = 120.f;  
    static constexpr float ACT_BTN_HEIGHT = 45.f;
    static constexpr float ACT_SPACING_X = 20.f;
    static constexpr float ACT_SPACING_Y = 15.f;

    static constexpr float NUMPAD_START_Y = 270.f; 
    static constexpr float NUMPAD_SIZE = 80.f;     
    static constexpr float NUMPAD_SPACING = 10.f;

    static constexpr float NEW_GAME_START_Y = 580.f; 
    static constexpr float NEW_GAME_WIDTH = 260.f;   
    static constexpr float NEW_GAME_HEIGHT = 50.f;
};

}

#endif