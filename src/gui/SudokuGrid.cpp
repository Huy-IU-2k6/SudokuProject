#include "SudokuGrid.h"
#include <string>

// Khởi tạo các hằng số màu sắc tĩnh
const sf::Color SudokuGrid::LINE_COLOR = sf::Color::Black;
const sf::Color SudokuGrid::TEXT_NORMAL_COLOR = sf::Color(50, 50, 50); // Màu xám tối thanh lịch
const sf::Color SudokuGrid::TEXT_WRONG_COLOR = sf::Color::Red;

SudokuGrid::SudokuGrid(const Board& board, const sf::Font& font)
    : mBoard(board)
    , mFont(font) 
{
    // Không chứa logic phức tạp ở constructor để giữ code sạch
}

void SudokuGrid::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // Áp dụng ma trận biến đổi Affine từ sf::Transformable
    states.transform *= getTransform();

    // Thực thi hai bước vẽ độc lập rạch ròi
    drawLines(target, states);
    drawNumbers(target, states);
}

void SudokuGrid::drawLines(sf::RenderTarget& target, sf::RenderStates states) const {
    const float totalGridLength = GRID_SIZE * CELL_SIZE;

    for (int i = 0; i <= GRID_SIZE; ++i) {
        sf::RectangleShape line;
        line.setFillColor(LINE_COLOR);

        // Xác định độ dày đường kẻ dựa trên ranh giới khối 3x3
        const float thickness = (i % SUBGRID_SIZE == 0) ? THICK_LINE : THIN_LINE;
        const float halfThickness = thickness / 2.f;

        // 1. Khởi tạo và vẽ các đường ngang (Horizontal Lines)
        line.setSize({totalGridLength, thickness});
        line.setPosition({0.f, (i * CELL_SIZE) - halfThickness});
        target.draw(line, states);

        // 2. Khởi tạo và vẽ các đường dọc (Vertical Lines)
        line.setSize({thickness, totalGridLength});
        line.setPosition({(i * CELL_SIZE) - halfThickness, 0.f});
        target.draw(line, states);
    }
}

void SudokuGrid::drawNumbers(sf::RenderTarget& target, sf::RenderStates states) const {
    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE; ++col) {
            
            Cell cell = mBoard.getCell(row, col);

            // Kiểm tra an toàn bằng std::optional (C++17) thay vì so sánh với số 0
            if (cell.value.has_value()) {
                sf::Text text(mFont);
                text.setString(std::to_string(cell.value.value()));
                text.setCharacterSize(FONT_SIZE);

                // Quản lý màu sắc dựa trên trạng thái logic của ô dữ liệu
                if (cell.is_wrong) {
                    text.setFillColor(TEXT_WRONG_COLOR);
                } else {
                    text.setFillColor(TEXT_NORMAL_COLOR);
                }

                // THUẬT TOÁN ĐỘNG CĂN TÂM CHỮ VÀO GIỮA Ô VUÔNG (DYNAMIC CENTERING)
                // Lấy khung bao thực tế của ký tự trong bộ nhớ đồ họa
                sf::FloatRect textBounds = text.getLocalBounds();

                // Đặt tâm cục bộ (Origin) của đối tượng Text vào chính giữa khung bao của nó
               // THUẬT TOÁN ĐỘNG CĂN TÂM CHỮ VÀO GIỮA Ô VUÔNG (Cú pháp SFML 3.1.0)


                text.setOrigin({
                                textBounds.position.x + (textBounds.size.x / 2.f),
                                textBounds.position.y + (textBounds.size.y / 2.f)
                            });

                // Tính toán tọa độ tâm hình học của ô vuông trên lưới
                const float cellCenterX = (col * CELL_SIZE) + (CELL_SIZE / 2.f);
                const float cellCenterY = (row * CELL_SIZE) + (CELL_SIZE / 2.f);

                // Đặt vị trí đối tượng về tâm ô vuông (Ma trận sẽ tự động xử lý phần còn lại)
                text.setPosition({cellCenterX, cellCenterY});

                target.draw(text, states);
            }
        }
    }
}