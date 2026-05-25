#include "core/board.h"

Board::Board() {
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            grid[row][col].value = std::nullopt; 
            grid[row][col].is_wrong = false;
        }
    }
}

Cell Board::getCell(int row, int col) const {
    return grid[row][col];
}

void Board::setCellValue(int row, int col, int val) {
    if (row >= 0 && row < 9 && col >= 0 && col < 9) {
        // TỐI ƯU: Nếu số điền vào giống hệt số cũ thì bỏ qua, không cần lưu Stack
        if (grid[row][col].value == val) return;

        // 1. PUSH VÀO STACK: Lưu lại giá trị cũ trước khi bị đổi
        mUndoStack.push_back({row, col, grid[row][col].value});

        // 2. Cập nhật giá trị mới
        grid[row][col].value = val;
    }
}

void Board::clearCell(int row, int col) {
    if (row >= 0 && row < 9 && col >= 0 && col < 9) {
        // TỐI ƯU: Nếu ô đã trống sẵn rồi thì xóa làm gì nữa, không cần lưu
        if (!grid[row][col].value.has_value()) return;

        // 1. PUSH VÀO STACK: Lưu lại giá trị trước khi bị xóa
        mUndoStack.push_back({row, col, grid[row][col].value});

        // 2. Đưa về khoảng trắng
        grid[row][col].value = std::nullopt; 
        grid[row][col].is_wrong = false;
    }
}

// LOGIC HOÀN TÁC (UNDO)
void Board::undo() {
    // Nếu Stack trống (chưa đi nước nào) thì không làm gì cả
    if (mUndoStack.empty()) return; 

    // Lấy nước đi gần nhất trên đỉnh Stack ra (Top)
    MoveRecord lastMove = mUndoStack.back();
    
    // Xóa nó khỏi Stack (Pop)
    mUndoStack.pop_back();

    // Khôi phục lại giá trị cũ của ô đó
    grid[lastMove.row][lastMove.col].value = lastMove.prevValue;
    grid[lastMove.row][lastMove.col].is_wrong = false; // Reset trạng thái lỗi
}

void Board::clearBoard() {
    // 1. Quét toàn bộ ma trận 9x9 để xóa hết số
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            grid[row][col].value = std::nullopt; 
            grid[row][col].is_wrong = false;
        }
    }
    
    // 2. GIẢI PHÓNG BỘ NHỚ STACK: Xóa sạch lịch sử các nước đi của ván cũ
    mUndoStack.clear(); 
}