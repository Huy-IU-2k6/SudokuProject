#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

namespace GUI {

class Button : public sf::Drawable, public sf::Transformable {
public:
    // Sử dụng Kiểu alias để quản lý hàm hành động khi click nút
    using Callback = std::function<void()>;

    // Constructor bắt buộc truyền Font chữ để hiển thị Label
    Button(const sf::Font& font);

    // Các hàm thiết lập thuộc tính trực quan (Setter)
    void setText(const std::string& text);
    void setCallback(Callback callback);
    void setButtonSize(sf::Vector2f size);
    
    // Hàm cập nhật trạng thái Hover và xử lý sự kiện click từ chuột
    void update(const sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);

private:
    // Ghi đè hàm vẽ đồ họa của lớp cha sf::Drawable
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
    // Hàm nội bộ tự động căn giữa chữ vào lòng nút bấm
    void centerText();

private:
    sf::RectangleShape mBackground;
    sf::Text mText;
    Callback mCallback;
    bool mIsHovered;

    // PALETTE MÀU SẮC GIAO DIỆN (Loại bỏ hoàn toàn Magic Colors/Numbers)
    static const sf::Color COLOR_NORMAL_BG;
    static const sf::Color COLOR_HOVER_BG;
    static const sf::Color COLOR_TEXT;
    static const sf::Color COLOR_OUTLINE;
    
    static constexpr float OUTLINE_THICKNESS = 2.f;
    static constexpr unsigned int DEFAULT_FONT_SIZE = 24;
};

}

#endif