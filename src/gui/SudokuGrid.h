#ifndef SUDOKU_GRID_H
#define SUDOKU_GRID_H

#include <SFML/Graphics.hpp>
#include "../core/board.h"

class SudokuGrid : public sf::Drawable, public sf::Transformable {
public:
    // Constructor nhận tham chiếu const để bảo vệ tính toàn vẹn dữ liệu
    SudokuGrid(const Board& board, const sf::Font& font);
    
    // Hàm nhận diện sự kiện click chuột
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void update(const sf::RenderWindow& window);
    // THÊM 2 HÀM NÀY ĐỂ BÁO TỌA ĐỘ CHO PLAYING_STATE
    int getSelectedRow() const { return mSelectedRow; }
    int getSelectedCol() const { return mSelectedCol; }

    // THÊM HÀM NÀY: Mở khóa để PlayingState có thể ra lệnh tắt Highlight
    void clearSelection() { mSelectedRow = -1; mSelectedCol = -1; }

private:
    // Ghi đè hàm draw bắt buộc
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    // Các hàm trợ năng (Tách biệt tác vụ vẽ rất tốt)
    void drawBackgrounds(sf::RenderTarget& target, sf::RenderStates states) const; // <-- THÊM MỚI
    void drawLines(sf::RenderTarget& target, sf::RenderStates states) const;
    void drawNumbers(sf::RenderTarget& target, sf::RenderStates states) const;

    const Board& mBoard;
    const sf::Font& mFont;

    // Tọa độ ô đang được click (-1 nghĩa là chưa click ô nào)
    int mSelectedRow;
    int mSelectedCol;

    static constexpr float CELL_SIZE = 60.f;
    static constexpr float THICK_LINE = 4.f;
    static constexpr float THIN_LINE = 1.f;
    static constexpr unsigned int FONT_SIZE = 36;
    static constexpr int GRID_SIZE = 9;
    static constexpr int SUBGRID_SIZE = 3;

    // Bảng màu chuẩn
    static const sf::Color LINE_COLOR;
    static const sf::Color TEXT_NORMAL_COLOR;
    static const sf::Color TEXT_WRONG_COLOR;
    static const sf::Color COLOR_BG_NORMAL;     
    static const sf::Color COLOR_BG_HIGHLIGHT;  
    static const sf::Color COLOR_BG_SELECTED;   
};

#endif