#include "core/board.h"

Board::Board() {
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            grid[row][col].value = std::nullopt; // Khai báo rõ ràng là ô trống
            grid[row][col].is_wrong = false;
        }
    }

    // Dữ liệu test hiển thị
    grid[0][0].value = 3;  // std::optional tự động bọc số 3 lại
    grid[0][0].is_wrong = true;
}

Cell Board::getCell(int row, int col) const {
    return grid[row][col];
}