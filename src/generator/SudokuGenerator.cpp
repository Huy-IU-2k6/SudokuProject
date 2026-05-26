#include "SudokuGenerator.h"
#include <random>
#include <algorithm>
#include <chrono>
#include <iostream>

static thread_local std::mt19937 s_rng(std::chrono::system_clock::now().time_since_epoch().count());

std::future<Board> SudokuGenerator::generateAsync(std::string difficulty) {
    return std::async(std::launch::async, [difficulty]() {
        return generate(difficulty);
    });
}

Board SudokuGenerator::generate(std::string difficulty) {
    int minScore, maxScore;
    getTargetScoreRange(difficulty, minScore, maxScore);

    int maxRetries = 50; 
    Board bestBoard;
    int bestScore = -1;

    while (maxRetries-- > 0) {
        Board board;
        
        // CHỈ GỌI 1 LẦN DUY NHẤT: Nếu AI điền bảng thất bại, bỏ đi làm lại ván khác!
        if (!fillBoard(board)) {
            continue; 
        }

        // BẢO VỆ TUYỆT ĐỐI: Dùng value_or(0) để C++ không bao giờ ném lỗi Crash
        int solutionCache[9][9] = {0};
        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c) {
                solutionCache[r][c] = board.getCell(r, c).value.value_or(0);
            }
        }

        std::vector<std::pair<int, int>> cells;
        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c) {
                cells.push_back({r, c});
            }
        }
        std::shuffle(cells.begin(), cells.end(), s_rng); 

        for (auto pos : cells) {
            int r = pos.first;
            int c = pos.second;
            
            auto cellOpt = board.getCell(r, c).value;
            if (!cellOpt.has_value()) continue;
            
            // BẢO VỆ: Dùng value_or
            int backupVal = cellOpt.value_or(0);
            
            board.clearCell(r, c);

            if (countSolutions(board) != 1) {
                board.setCellValue(r, c, backupVal); 
                continue; 
            }

            Board copyBoard = board;
            SolverStats stats;
            SudokuSolver::solve(copyBoard, stats); 
            int currentScore = stats.calculateDifficultyScore();

            if (currentScore > bestScore) {
                bestScore = currentScore;
                bestBoard = board;
                
                // Đảm bảo bảng dự phòng cũng có đáp án để xài nút Hint
                for (int i = 0; i < 9; ++i) {
                    for (int j = 0; j < 9; ++j) {
                        bestBoard.setSolutionValue(i, j, solutionCache[i][j]);
                    }
                }
            }

            if (currentScore >= minScore && currentScore <= maxScore) {
                for (int i = 0; i < 9; ++i) {
                    for (int j = 0; j < 9; ++j) {
                        board.setSolutionValue(i, j, solutionCache[i][j]);
                    }
                }
                board.prepareGame(); 
                return board; 
            }
            
            if (currentScore > maxScore) {
                board.setCellValue(r, c, backupVal);
            }
        }
    }
    
    bestBoard.prepareGame();
    return bestBoard;
}

bool SudokuGenerator::fillBoard(Board& board) {
    int r = -1, c = -1;
    bool empty = false;
    for (int i = 0; i < 9 && !empty; ++i) {
        for (int j = 0; j < 9 && !empty; ++j) {
            if (!board.getCell(i, j).value.has_value()) {
                r = i; c = j; empty = true;
            }
        }
    }
    if (!empty) return true; 

    std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::shuffle(nums.begin(), nums.end(), s_rng);

    for (int num : nums) {
        if (isValid(board, r, c, num)) {
            board.setCellValue(r, c, num);
            if (fillBoard(board)) return true;
            board.clearCell(r, c);
        }
    }
    return false;
}

namespace {
    inline int countBits(uint16_t n) {
        int count = 0;
        while (n) { n &= (n - 1); count++; }
        return count;
    }

    bool propagateAC3(uint16_t domains[81], std::vector<int>& queue) {
        while (!queue.empty()) {
            int curr = queue.back();
            queue.pop_back();
            
            uint16_t val = domains[curr];
            int r = curr / 9;
            int c = curr % 9;
            int b = (r / 3) * 3 + (c / 3);

            int peers[24];
            int peerCount = 0;
            for (int i = 0; i < 9; ++i) {
                int pr = r, pc = i;
                if (pc != c) peers[peerCount++] = pr * 9 + pc; 
                pr = i, pc = c;
                if (pr != r) peers[peerCount++] = pr * 9 + pc; 
                pr = (b / 3) * 3 + (i / 3);
                pc = (b % 3) * 3 + (i % 3);
                if (pr != r && pc != c) peers[peerCount++] = pr * 9 + pc; 
            }

            for (int i = 0; i < peerCount; ++i) {
                int peer = peers[i];
                if (domains[peer] & val) { 
                    domains[peer] &= ~val; 
                    if (domains[peer] == 0) return false; 
                    if (countBits(domains[peer]) == 1) {
                        queue.push_back(peer); 
                    }
                }
            }
        }
        return true;
    }

    void fastCountSolutionsAC3(uint16_t domains[81], int& count) {
        if (count >= 2) return; 

        int bestCell = -1;
        int minOptions = 10;
        for (int i = 0; i < 81; ++i) {
            int opts = countBits(domains[i]);
            if (opts > 1 && opts < minOptions) {
                minOptions = opts;
                bestCell = i;
                if (opts == 2) break; 
            }
        }

        if (bestCell == -1) { 
            count++;
            return;
        }

        uint16_t available = domains[bestCell];
        for (int num = 1; num <= 9; ++num) {
            int bit = 1 << (num - 1);
            if (available & bit) {
                uint16_t nextDomains[81];
                std::copy(domains, domains + 81, nextDomains);
                
                nextDomains[bestCell] = bit;
                std::vector<int> q;
                q.push_back(bestCell);
                
                if (propagateAC3(nextDomains, q)) {
                    fastCountSolutionsAC3(nextDomains, count);
                }
                if (count >= 2) return; 
            }
        }
    }
}

int SudokuGenerator::countSolutions(Board board) {
    uint16_t domains[81];
    for (int i = 0; i < 81; ++i) domains[i] = 0x1FF; 

    std::vector<int> queue;
    
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (board.getCell(r, c).value.has_value()) {
                int idx = r * 9 + c;
                // BẢO VỆ: Dùng value_or(1)
                int val = board.getCell(r, c).value.value_or(1);
                domains[idx] = 1 << (val - 1);
                queue.push_back(idx);
            }
        }
    }

    if (!propagateAC3(domains, queue)) return 0; 

    int count = 0;
    fastCountSolutionsAC3(domains, count);
    return count;
}

void SudokuGenerator::solveAndCount(Board& board, int& count) {
    count = countSolutions(board); 
}

bool SudokuGenerator::isValid(const Board& board, int r, int c, int num) {
    for (int i = 0; i < 9; ++i) {
        if (board.getCell(r, i).value == num) return false;
        if (board.getCell(i, c).value == num) return false;
    }
    int br = (r / 3) * 3;
    int bc = (c / 3) * 3;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board.getCell(br + i, bc + j).value == num) return false;
        }
    }
    return true;
}

void SudokuGenerator::getTargetScoreRange(const std::string& difficulty, int& minScore, int& maxScore) {
    if (difficulty == "Easy") { minScore = 450; maxScore = 600; }
    else if (difficulty == "Medium") { minScore = 650; maxScore = 800; }
    else if (difficulty == "Hard") { minScore = 850; maxScore = 1100; }
    else if (difficulty == "Expert") { minScore = 1200; maxScore = 2000; }
    else if (difficulty == "Master") { minScore = 2200; maxScore = 5000; }
    else if (difficulty == "Extreme") { minScore = 5001; maxScore = 999999; }
    else { minScore = 450; maxScore = 600; } 
}