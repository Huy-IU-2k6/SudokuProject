#include "core/board.h"

Board::Board() {
    clearBoard();
}

Cell Board::getCell(int row, int col) const {
    return grid[row][col];
}

void Board::setCellValue(int row, int col, int val) {
    if (row >= 0 && row < 9 && col >= 0 && col < 9) {
        // NGĂN CHẶN: Nếu là ô cố định của đề bài thì tuyệt đối không cho ghi đè
        if (grid[row][col].is_fixed) return; 
        if (grid[row][col].value == val) return;

        // Lưu trạng thái trước khi thay đổi vào Stack
        mUndoStack.push_back({row, col, grid[row][col].value});
        grid[row][col].value = val;
    }
}

void Board::clearCell(int row, int col) {
    if (row >= 0 && row < 9 && col >= 0 && col < 9) {
        // NGĂN CHẶN: Không cho phép dùng cục tẩy xóa số mặc định của đề bài
        if (grid[row][col].is_fixed) return; 
        if (!grid[row][col].value.has_value()) return;

        // Lưu trạng thái trước khi xóa vào Stack
        mUndoStack.push_back({row, col, grid[row][col].value});
        grid[row][col].value = std::nullopt; 
        grid[row][col].is_wrong = false;
    }
}

void Board::undo() {
    if (mUndoStack.empty()) return; 

    // Lấy nước đi gần nhất trên đỉnh Stack ra và loại bỏ nó khỏi ngăn xếp
    MoveRecord lastMove = mUndoStack.back();
    mUndoStack.pop_back();

    // Khôi phục lại giá trị nguyên bản
    grid[lastMove.row][lastMove.col].value = lastMove.prevValue;
    grid[lastMove.row][lastMove.col].is_wrong = false;
}

void Board::clearBoard() {
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            grid[row][col].value = std::nullopt; 
            grid[row][col].is_fixed = false;
            grid[row][col].is_wrong = false;
        }
    }
    mUndoStack.clear();
}

void Board::prepareGame() {
    // 1. Quét qua toàn bộ ma trận, ô nào đang chứa số thì khóa cứng lại
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (grid[row][col].value.has_value()) {
                grid[row][col].is_fixed = true;
            }
        }
    }
    // 2. XÓA SẠCH VẾT TÍNH TOÁN CỦA AI: Trả lại một Stack trống hoàn toàn cho người chơi
    mUndoStack.clear(); 
}