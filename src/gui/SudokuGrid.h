#ifndef SUDOKU_GRID_H
#define SUDOKU_GRID_H

#include <SFML/Graphics.hpp>
#include "../core/board.h"

class SudokuGrid : public sf::Drawable, public sf::Transformable {
public:
    // Constructor nhận tham chiếu const để bảo vệ tính toàn vẹn dữ liệu của Tầng Core
    SudokuGrid(const Board& board, const sf::Font& font);

private:
    // Ghi đè hàm draw bắt buộc của giao diện sf::Drawable
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    // Các hàm trợ năng nội bộ (Helper Functions) để phân tách tác vụ vẽ
    void drawLines(sf::RenderTarget& target, sf::RenderStates states) const;
    void drawNumbers(sf::RenderTarget& target, sf::RenderStates states) const;

    // Tham chiếu đến các tài nguyên bên ngoài
    const Board& mBoard;
    const sf::Font& mFont;

    // CONFIGURATION CONSTANTS (Xóa bỏ hoàn toàn Magic Numbers)
    static constexpr float CELL_SIZE = 60.f;
    static constexpr float THICK_LINE = 4.f;
    static constexpr float THIN_LINE = 1.f;
    static constexpr unsigned int FONT_SIZE = 36;
    static constexpr int GRID_SIZE = 9;
    static constexpr int SUBGRID_SIZE = 3;

    // Bảng màu chuẩn cho UI (Color Palette)
    static const sf::Color LINE_COLOR;
    static const sf::Color TEXT_NORMAL_COLOR;
    static const sf::Color TEXT_WRONG_COLOR;
};

#endif