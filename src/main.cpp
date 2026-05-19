#include <SFML/Graphics.hpp>
#include "core/board.h"
#include <iostream>

int main() {
    // 1. CÚ PHÁP MỚI: Khởi tạo VideoMode bằng sf::Vector2u thay vì (800, 600)
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Sudoku Platform");
    window.setFramerateLimit(60);

    Board board; 
    sf::Font font;
    
    // 2. CÚ PHÁP MỚI: SFML 3.x đã đổi tên loadFromFile thành openFromFile
    if (!font.openFromFile("assets/fonts/arial.ttf")) {
        std::cout << "Loi! Khong load duoc font chu." << std::endl;
        return -1;
    }

    // 3. Vòng lặp Game Loop Modern kiểu mới của SFML 3.x
    while (window.isOpen()) {
        
        // 4. CÚ PHÁP MỚI: pollEvent trả về std::optional ở SFML 3.x, không dùng kiểu truyền reference cũ
        while (const std::optional event = window.pollEvent()) {
            // Check sự kiện đóng cửa sổ
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color(240, 240, 240));

        // Lấy ô lỗi từ Core ra để hiển thị test màu đỏ
        Cell testCell = board.getCell(0, 1); 
        
        // 5. CÚ PHÁP MỚI: Constructor của sf::Text bắt buộc phải nhận Font ngay từ đầu
        sf::Text text(font);
        text.setString(std::to_string(testCell.value));
        text.setCharacterSize(50);
        
        if (testCell.is_wrong) {
            text.setFillColor(sf::Color::Red); // Sai tô màu đỏ
        } else {
            text.setFillColor(sf::Color::Black);
        }
        
        // 6. CÚ PHÁP MỚI: setPosition nhận sf::Vector2f, viết gọn bằng dấu ngoặc nhọn {x, y}
        text.setPosition({100.f, 100.f});
        
        window.draw(text);
        window.display();
    }
    return 0;
}