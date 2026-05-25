#include "core/board.h"

Board::Board() {
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            grid[row][col].value = std::nullopt; // Khai báo rõ ràng là ô trống
            grid[row][col].is_wrong = false;
        }
    }


}

Cell Board::getCell(int row, int col) const {
    return grid[row][col];
}