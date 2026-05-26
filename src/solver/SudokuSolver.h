#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

#include "../core/board.h"
#include <cstdint> // Su dung uint16_t cho Bitmask

struct SolverStats {
    int recursionCalls = 0;
    int backtrackCount = 0;
    int maxDepth = 0;

    // Tinh diem do kho dua tren cac trong so hoc thuat
    int calculateDifficultyScore() const {
        return (recursionCalls * 1) + (backtrackCount * 8) + (maxDepth * 15);
    }
};

class SudokuSolver {
public:
    // Ham chinh de giai board va tra ve thong ke
    static bool solve(Board& board, SolverStats& stats);
    
    // Ham dem so nghiem de phuc vu cho viec Generate (Kiem tra Unique)
    static int countSolutions(Board board);

private:
    // Thuat toan Backtracking loi (Core)
    static bool backtrack(Board& board, uint16_t rows[], uint16_t cols[], uint16_t blocks[], int depth, SolverStats& stats);
    
    // Heuristic MRV: Tim o trong co it lua chon nhat
    static bool findMRVCell(const Board& board, const uint16_t rows[], const uint16_t cols[], const uint16_t blocks[], int& bestRow, int& bestCol);
    
    // Helper: Tinh vi tri khoi 3x3
    static int getBlockIndex(int r, int c) {
        return (r / 3) * 3 + (c / 3);
    }
};

#endif