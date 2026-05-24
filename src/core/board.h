#ifndef BOARD_H
#define BOARD_H

#include <optional> // Thư viện chứa std::optional (C++17)

struct Cell {
    std::optional<int> value; // Chứa int hoặc không chứa gì cả (nullopt)
    bool is_wrong;
};

class Board {
public:
    Board();
    Cell getCell(int row, int col) const;

private:
    Cell grid[9][9];
};

#endif