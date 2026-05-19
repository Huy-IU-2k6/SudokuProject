#include "board.h"
#include <iostream>

Board::Board() {
    initDummyBoard();
}

void Board::initDummyBoard() {
    // Thử điền vài số mặc định để test xem lên hình đúng không
    grid[0][0].value = 5;
    grid[0][0].is_fixed = true;

    grid[0][1].value = 3;
    grid[0][1].is_wrong = true; // Thử nghiệm ô điền sai để GUI vẽ màu đỏ
}

Cell Board::getCell(int r, int c) const {
    return grid[r][c];
}