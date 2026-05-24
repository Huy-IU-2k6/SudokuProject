#include "Button.h"

namespace GUI {

// Định nghĩa bảng màu sắc đồng bộ cho hệ thống UI
const sf::Color Button::COLOR_NORMAL_BG  = sf::Color(245, 245, 245); // Màu nền trắng sáng
const sf::Color Button::COLOR_HOVER_BG   = sf::Color(220, 230, 242); // Màu xanh dương nhạt khi rê chuột
const sf::Color Button::COLOR_TEXT       = sf::Color(40, 40, 40);     // Màu chữ xám tối thanh lịch
const sf::Color Button::COLOR_OUTLINE    = sf::Color(160, 160, 160);  // Màu đường viền bao

Button::Button(const sf::Font& font)
    : mBackground()
    , mText(font)
    , mCallback(nullptr)
    , mIsHovered(false)
{
    // Cấu hình ban đầu cho hình khối nền của Nút
    mBackground.setFillColor(COLOR_NORMAL_BG);
    mBackground.setOutlineColor(COLOR_OUTLINE);
    mBackground.setOutlineThickness(OUTLINE_THICKNESS);

    // Cấu hình ban đầu cho nhãn chữ
    mText.setCharacterSize(DEFAULT_FONT_SIZE);
    mText.setFillColor(COLOR_TEXT);
}

void Button::setText(const std::string& text) {
    mText.setString(text);
    centerText(); // Mỗi lần đổi chữ phải tính toán lại tâm hình học
}

void Button::setButtonSize(sf::Vector2f size) {
    mBackground.setSize(size);
    centerText(); // Mỗi lần đổi kích thước nút phải căn chỉnh lại vị trí chữ
}

void Button::setCallback(Callback callback) {
    mCallback = std::move(callback);
}

void Button::update(const sf::RenderWindow& window) {
    // Thuật toán kiểm tra va chạm điểm (Raycasting / Bounding Box) cho chuột
    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mouseCoords = window.mapPixelToCoords(mousePixelPos);

    // Áp dụng Ma trận biến đổi Affine hiện tại của thực thể để lấy khung bao toàn cục
    sf::FloatRect globalBounds = getTransform().transformRect(mBackground.getLocalBounds());

    mIsHovered = globalBounds.contains(mouseCoords);

    // Đổi màu mượt mà theo trạng thái tương tác
    if (mIsHovered) {
        mBackground.setFillColor(COLOR_HOVER_BG);
    } else {
        mBackground.setFillColor(COLOR_NORMAL_BG);
    }
}

void Button::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    // Kiểm tra xem sự kiện truyền vào có phải là click chuột trái hay không (Chuẩn SFML 3.1.0)
    if (const auto* mouseClick = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseClick->button == sf::Mouse::Button::Left && mIsHovered) {
            // Kích hoạt hàm gọi ngược nếu nút đã được gán hành động hợp lệ
            if (mCallback) {
                mCallback();
            }
        }
    }
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // Tích hợp ma trận dịch chuyển vị trí của sf::Transformable
    states.transform *= getTransform();
    
    // Vẽ tuần tự: Nền hiện trước, chữ đè lên sau
    target.draw(mBackground, states);
    target.draw(mText, states);
}

void Button::centerText() {
    sf::FloatRect textBounds = mText.getLocalBounds();
    sf::Vector2f bgSize = mBackground.getSize();

    // 1. Căn tâm cục bộ (Origin) của Text về chính giữa bounding box của ký tự (Chuẩn SFML 3)
    mText.setOrigin({
        textBounds.position.x + (textBounds.size.x / 2.f),
        textBounds.position.y + (textBounds.size.y / 2.f)
    });

    // 2. Đặt vị trí Text vào trung điểm hình học của RectangleShape nền
    mText.setPosition({
        bgSize.x / 2.f,
        bgSize.y / 2.f
    });
}

}