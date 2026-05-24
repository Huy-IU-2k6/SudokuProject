#ifndef GAME_UI_H
#define GAME_UI_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
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

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
    void initTexts();
    void initActionButtons();
    void initNumpad();
    void initNewGameButton(); // Tách nút New Game thành hàm riêng

private:
    const sf::Font& mFont;

    sf::Text mMistakesText;
    sf::Text mTimeText;

    std::vector<std::unique_ptr<Button>> mActionButtons;
    std::vector<std::unique_ptr<Button>> mNumpadButtons;
    std::unique_ptr<Button> mNewGameButton; // Đối tượng nút độc lập

    // --- KHÔNG GIAN BỐ TRÍ ĐỒ HỌA CHUẨN UX (XÓA BỎ CRAMMED LAYOUT) ---
    static constexpr float PANEL_START_X = 660.f;  // Dịch sang phải một chút tạo khoảng cách với lưới Sudoku
    static constexpr float PANEL_START_Y = 40.f;

    // Cấu hình lưới 2x2 cho các nút chức năng
    static constexpr float ACT_BTN_WIDTH = 120.f;  
    static constexpr float ACT_BTN_HEIGHT = 45.f;
    static constexpr float ACT_SPACING_X = 20.f;
    static constexpr float ACT_SPACING_Y = 15.f;

    // Cấu hình lưới 3x3 cho bàn phím số (Numpad)
    static constexpr float NUMPAD_START_Y = 270.f; // Đẩy khoảng cách đệm an toàn với cụm trên
    static constexpr float NUMPAD_SIZE = 80.f;     // Tăng kích thước phím lên 80px cho dễ bấm
    static constexpr float NUMPAD_SPACING = 10.f;

    // Cấu hình nút New Game ở đáy
    static constexpr float NEW_GAME_START_Y = 580.f; // Đẩy hẳn xuống đáy màn hình
    static constexpr float NEW_GAME_WIDTH = 260.f;   // Chiều rộng bao phủ trọn cụm UI
    static constexpr float NEW_GAME_HEIGHT = 50.f;
};

}

#endif