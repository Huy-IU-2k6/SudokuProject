#ifndef BOARD_H
#define BOARD_H

#include <optional> 
#include <vector> 

struct Cell {
    std::optional<int> value; 
    bool is_fixed = false; 
    bool is_wrong = false; 
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

    // Các hàm Gợi ý & Đáp án
    void setSolutionValue(int row, int col, int val); 
    void revealHint(int row, int col);
    void revealSolution();
    int getMistakes() const;

    // Các hàm Xử lý Thắng / Thua
    bool isSolved() const;     
    void useSecondChance();    

private:
    Cell grid[9][9];
    std::vector<MoveRecord> mUndoStack; 
    
    int mSolution[9][9] = {0}; 
    int mMistakes = 0;         
};

#endif