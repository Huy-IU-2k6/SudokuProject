#ifndef SUDOKU_GENERATOR_H
#define SUDOKU_GENERATOR_H

#include "../core/board.h"
#include "../solver/SudokuSolver.h"
#include <string>
#include <future>
#include <vector>

class SudokuGenerator {
public:
    // HÀM CHÍNH: Trả về một "Tương lai" (Future) chứa Board, chạy hoàn toàn bất đồng bộ (Async)
    static std::future<Board> generateAsync(std::string difficulty);

private:
    // Core flow chay ngam
    static Board generate(std::string difficulty);
    
    // Step 1: Dien day 81 o hop le bang Randomized Backtracking
    static bool fillBoard(Board& board);
    
    // Step 3: Kiem tra tinh duy nhat cua nghiem (Tra ve so luong nghiem)
    static int countSolutions(Board board); // Truyen tham tri de copy, tranh hong board goc
    static void solveAndCount(Board& board, int& count);
    
    // Helper kiem tra luat Sudoku
    static bool isValid(const Board& board, int r, int c, int num);
    
    // Helper anh xa Do kho thanh Diem so (Score)
    static void getTargetScoreRange(const std::string& difficulty, int& minScore, int& maxScore);
};

#endif