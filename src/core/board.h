#ifndef BOARD_H
#define BOARD_H

#include "cell.h"

class Board {
private:
    Cell grid[9][9];
    int solution[9][9]; // Dùng để đối chiếu đúng/sai cho tính năng báo lỗi đỏ

public:
    Board();
    void initDummyBoard(); // Tạo tạm một bảng để test giao diện
    Cell getCell(int r, int c) const;
};

#endif