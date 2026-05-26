#include "SudokuSolver.h"
#include <algorithm>

bool SudokuSolver::solve(Board& board, SolverStats& stats) {
    uint16_t rows[9] = {0}, cols[9] = {0}, blocks[9] = {0};

    // 1. Khoi tao trang thai Bitmask ban dau tu Board hien tai
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            auto cellVal = board.getCell(r, c).value;
            if (cellVal.has_value()) {
                int val = cellVal.value();
                int bit = 1 << (val - 1); // Dich bit tuong ung voi so (1-9)
                rows[r] |= bit;
                cols[c] |= bit;
                blocks[getBlockIndex(r, c)] |= bit;
            }
        }
    }

    // 2. Bat dau de quy
    return backtrack(board, rows, cols, blocks, 0, stats);
}

bool SudokuSolver::findMRVCell(const Board& board, const uint16_t rows[], const uint16_t cols[], const uint16_t blocks[], int& bestRow, int& bestCol) {
    int minCandidates = 10; 
    bool foundEmpty = false;

    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (!board.getCell(r, c).value.has_value()) {
                foundEmpty = true;
                
                // Phep toan Bitwise: OR 3 tap hop, sau do lay phu dinh (~), roi AND voi 0x1FF (9 bit 1)
                uint16_t used = rows[r] | cols[c] | blocks[getBlockIndex(r, c)];
                uint16_t available = ~used & 0x1FF; 

                // Dem nhanh so luong bit 1 (So ung cu vien co the dien vao o nay)
                int numCandidates = __builtin_popcount(available);

                // Cap nhat MRV
                if (numCandidates < minCandidates) {
                    minCandidates = numCandidates;
                    bestRow = r;
                    bestCol = c;
                    // Toi uu: Neu thay o nao chi co 1 lua chon (Naked Single), chon luon lap tuc!
                    if (numCandidates == 1) return true; 
                }
            }
        }
    }
    return foundEmpty;
}

bool SudokuSolver::backtrack(Board& board, uint16_t rows[], uint16_t cols[], uint16_t blocks[], int depth, SolverStats& stats) {
    stats.recursionCalls++;
    stats.maxDepth = std::max(stats.maxDepth, depth);

    int r = -1, c = -1;
    
    // 1. Ap dung Heuristic MRV de tim o toi uu nhat
    if (!findMRVCell(board, rows, cols, blocks, r, c)) {
        return true; // Khong con o trong -> Giai thanh cong!
    }

    int b = getBlockIndex(r, c);
    uint16_t used = rows[r] | cols[c] | blocks[b];
    uint16_t available = ~used & 0x1FF;

    // 2. Thu tung ung cu vien (Forward Checking)
    for (int num = 1; num <= 9; ++num) {
        int bit = 1 << (num - 1);
        
        // Neu so nay chua bi dung (bit == 1 trong mask available)
        if (available & bit) {
            // Danh dau trang thai
            board.setCellValue(r, c, num);
            rows[r] |= bit;
            cols[c] |= bit;
            blocks[b] |= bit;

            // De quy (Recurse)
            if (backtrack(board, rows, cols, blocks, depth + 1, stats)) {
                return true;
            }

            // Neu That bai -> Quay lui (Backtrack)
            stats.backtrackCount++;
            board.clearCell(r, c);
            rows[r] &= ~bit;
            cols[c] &= ~bit;
            blocks[b] &= ~bit;
        }
    }

    return false; // Phai thu nhanh khac
}