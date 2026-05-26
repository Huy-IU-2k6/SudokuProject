#ifndef BOARD_H
#define BOARD_H

#include <optional> 
#include <vector> 

struct Cell {
    std::optional<int> value; 
    bool is_fixed = false; // true nếu là số đề bài cho sẵn (khóa cứng không cho sửa/xóa)
    bool is_wrong = false; // true nếu người chơi điền sai luật
};

// Cấu trúc đóng gói dữ liệu phục vụ tính năng Hoàn tác
struct MoveRecord {
    int row;
    int col;
    std::optional<int> prevValue; // Giá trị cũ trước khi bị can thiệp
};

class Board {
public:
    Board();
    Cell getCell(int row, int col) const;
    
    void setCellValue(int row, int col, int val);
    void clearCell(int row, int col);
    
    void undo();       // Hoàn tác nước đi của người chơi
    void clearBoard(); // Đưa ma trận về trạng thái trống hoàn toàn
    void prepareGame(); // Khóa số đề bài và làm sạch lịch sử tạo bảng của AI

private:
    Cell grid[9][9];
    std::vector<MoveRecord> mUndoStack; // Ngăn xếp lưu lịch sử nước đi
};

#endif