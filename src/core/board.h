#ifndef BOARD_H
#define BOARD_H

#include <optional>
#include <vector> // THÊM THƯ VIỆN NÀY ĐỂ LÀM STACK

struct Cell {
    std::optional<int> value; 
    bool is_wrong;
};

// ĐÂY CHÍNH LÀ CẤU TRÚC LƯU LỊCH SỬ
struct MoveRecord {
    int row;
    int col;
    std::optional<int> prevValue; // Lưu lại giá trị TRƯỚC KHI bị ghi đè
};

class Board {
public:
    Board();
    Cell getCell(int row, int col) const;
    void setCellValue(int row, int col, int val);
    void clearCell(int row, int col);
    
    // THÊM HÀM UNDO VÀO ĐÂY
    void undo(); 
    void clearBoard();

private:
    Cell grid[9][9];
    
    // ĐÂY CHÍNH LÀ NGĂN XẾP (STACK) CỦA BẠN
    std::vector<MoveRecord> mUndoStack; 
};

#endif