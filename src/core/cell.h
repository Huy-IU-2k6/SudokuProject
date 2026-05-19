#ifndef CELL_H
#define CELL_H

struct Cell {
    int value = 0;         // 0 nghĩa là ô trống
    bool is_fixed = false; // true nếu là số đề bài cho sẵn (không được sửa)
    bool is_wrong = false; // true nếu user điền sai so với đáp án (in màu đỏ)
};

#endif