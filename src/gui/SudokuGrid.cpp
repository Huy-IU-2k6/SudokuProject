#include "SudokuGrid.h"
#include <string>

// Khởi tạo các hằng số màu sắc tĩnh
const sf::Color SudokuGrid::LINE_COLOR = sf::Color::Black;
const sf::Color SudokuGrid::TEXT_NORMAL_COLOR = sf::Color(50, 50, 50); // Màu xám tối thanh lịch
const sf::Color SudokuGrid::TEXT_WRONG_COLOR = sf::Color::Red;
const sf::Color SudokuGrid::COLOR_BG_NORMAL    = sf::Color::White;
const sf::Color SudokuGrid::COLOR_BG_HIGHLIGHT = sf::Color(226, 235, 243); // Xanh nhạt
const sf::Color SudokuGrid::COLOR_BG_SELECTED  = sf::Color(170, 210, 245); // Xanh đậm

SudokuGrid::SudokuGrid(const Board& board, const sf::Font& font)
    : mBoard(board)
    , mFont(font)
    , mSelectedRow(-1) // Khởi tạo mặc định chưa chọn ô nào
    , mSelectedCol(-1)
{
    // Không chứa logic phức tạp ở constructor để giữ code sạch
}

// THUẬT TOÁN BẮT TỌA ĐỘ CHUỘT
void SudokuGrid::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (const auto* mouseClick = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseClick->button == sf::Mouse::Button::Left) {
            
            sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
            sf::Vector2f mouseCoords = window.mapPixelToCoords(mousePixelPos);

            // Dịch tọa độ chuột về hệ tọa độ bên trong Lưới Sudoku
            sf::Transform inverseTransform = getInverseTransform();
            sf::Vector2f localPos = inverseTransform.transformPoint(mouseCoords);

            float totalGridSize = GRID_SIZE * CELL_SIZE;

            // Nếu click nằm trong phạm vi của Lưới
            if (localPos.x >= 0.f && localPos.x < totalGridSize &&
                localPos.y >= 0.f && localPos.y < totalGridSize) 
            {
                mSelectedCol = static_cast<int>(localPos.x / CELL_SIZE);
                mSelectedRow = static_cast<int>(localPos.y / CELL_SIZE);
            } 
        }
    }
}

void SudokuGrid::update(const sf::RenderWindow& window) {
    // Dành cho animation nếu cần sau này
}

void SudokuGrid::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // Áp dụng ma trận biến đổi Affine từ sf::Transformable
    states.transform *= getTransform();

    // NGUYÊN LÝ HỌA SĨ (Vẽ từ dưới lên trên): Nền -> Kẻ vạch -> Viết chữ
    drawBackgrounds(target, states);
    drawLines(target, states);
    drawNumbers(target, states);
}

// THUẬT TOÁN ĐỔ MÀU HIGHLIGHT KHỐI LOGIC
void SudokuGrid::drawBackgrounds(sf::RenderTarget& target, sf::RenderStates states) const {
    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE; ++col) {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            cell.setPosition({col * CELL_SIZE, row * CELL_SIZE});

            // Nếu ô hiện tại đang được click
            if (row == mSelectedRow && col == mSelectedCol) {
                cell.setFillColor(COLOR_BG_SELECTED);
            } 
            // Nếu ô hiện tại cùng hàng, cùng cột, hoặc cùng khối 3x3 với ô được click
            else if (mSelectedRow != -1 && mSelectedCol != -1 && 
                    (row == mSelectedRow || col == mSelectedCol || 
                    (row / SUBGRID_SIZE == mSelectedRow / SUBGRID_SIZE && col / SUBGRID_SIZE == mSelectedCol / SUBGRID_SIZE))) {
                cell.setFillColor(COLOR_BG_HIGHLIGHT);
            } 
            // Ô bình thường
            else {
                cell.setFillColor(COLOR_BG_NORMAL);
            }

            target.draw(cell, states);
        }
    }
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