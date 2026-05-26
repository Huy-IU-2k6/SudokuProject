#include "core/board.h"

Board::Board() {
    clearBoard();
}

Cell Board::getCell(int row, int col) const {
    return grid[row][col];
}

void Board::setCellValue(int row, int col, int val) {
    if (row >= 0 && row < 9 && col >= 0 && col < 9) {
        if (grid[row][col].is_fixed) return; 
        if (grid[row][col].value == val) return;

        mUndoStack.push_back({row, col, grid[row][col].value});
        grid[row][col].value = val;
        
        // KIỂM TRA ĐÁP ÁN: Khác đáp án thì bật cờ đỏ và cộng lỗi
        if (val != mSolution[row][col]) {
            grid[row][col].is_wrong = true;
            mMistakes++;
        } else {
            grid[row][col].is_wrong = false;
        }
    }
}

void Board::clearCell(int row, int col) {
    if (row >= 0 && row < 9 && col >= 0 && col < 9) {
        if (grid[row][col].is_fixed) return; 
        if (!grid[row][col].value.has_value()) return;

        mUndoStack.push_back({row, col, grid[row][col].value});
        grid[row][col].value = std::nullopt; 
        grid[row][col].is_wrong = false;
    }
}

void Board::undo() {
    if (mUndoStack.empty()) return; 

    MoveRecord lastMove = mUndoStack.back();
    mUndoStack.pop_back();

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
    mMistakes = 0; // RESET LỖI
}

void Board::prepareGame() {
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (grid[row][col].value.has_value()) {
                grid[row][col].is_fixed = true;
                // TẨY TRẮNG 1: Đề bài mặc định thì chắc chắn phải đúng, không thể bị đỏ!
                grid[row][col].is_wrong = false;
            }
        }
    }
    mUndoStack.clear(); 
    // TẨY TRẮNG 3: Reset toàn bộ án tích (số lỗi) AI vừa gây ra về 0
    mMistakes = 0;
}

void Board::setSolutionValue(int row, int col, int val) {
    mSolution[row][col] = val;
}

void Board::revealHint(int row, int col) {
    if (row >= 0 && row < 9 && col >= 0 && col < 9) {
        if (grid[row][col].is_fixed) return; 
        
        mUndoStack.push_back({row, col, grid[row][col].value}); 
        grid[row][col].value = mSolution[row][col]; 
        grid[row][col].is_wrong = false;
        grid[row][col].is_fixed = true; 
    }
}

void Board::revealSolution() {
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (!grid[row][col].is_fixed) {
                grid[row][col].value = mSolution[row][col];
                grid[row][col].is_wrong = false;
                grid[row][col].is_fixed = true; 
            }
        }
    }
}

int Board::getMistakes() const {
    return mMistakes;
}