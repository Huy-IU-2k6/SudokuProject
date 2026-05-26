#ifndef BOARD_H
#define BOARD_H

#include <optional> 
#include <vector> 

struct Cell {
    std::optional<int> value; 
    bool is_fixed = false; // true nếu là số đề bài cho sẵn (khóa cứng)
    bool is_wrong = false; // true nếu người chơi điền sai luật
};

struct MoveRecord {
    int row;
    int col;
    std::optional<int> prevValue; 
};

class Board {
public:
    Board();
    Cell getCell(int row, int col) const;
    
    void setCellValue(int row, int col, int val);
    void clearCell(int row, int col);
    
    void undo();       
    void clearBoard(); 
    void prepareGame(); 

    // 3 HÀM MỚI PHỤC VỤ HINT VÀ SOLUTION
    void setSolutionValue(int row, int col, int val); 
    void revealHint(int row, int col);
    void revealSolution();
    int getMistakes() const;

private:
    Cell grid[9][9];
    std::vector<MoveRecord> mUndoStack; 
    
    int mSolution[9][9] = {0}; // Mảng bí mật chứa đáp án
    int mMistakes = 0;         // Bộ đếm lỗi
};

#endif