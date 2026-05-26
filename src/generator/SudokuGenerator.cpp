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

    // Đã có AC-3, tốc độ tính bằng mili-giây nên ta tăng Retries lên 50 để AI thoải mái tìm bảng Master
    int maxRetries = 50; 
    Board bestBoard;
    int bestScore = -1;

    while (maxRetries-- > 0) {
        Board board;
        fillBoard(board);

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
            int backupVal = board.getCell(r, c).value.value();

            board.clearCell(r, c);

            // BỘ ĐẾM NGHIỆM AC-3 HOẠT ĐỘNG TẠI ĐÂY
            if (countSolutions(board) != 1) {
                board.setCellValue(r, c, backupVal); 
                continue; 
            }

            Board copyBoard = board;
            SolverStats stats;
            SudokuSolver::solve(copyBoard, stats); // Solver mô phỏng người chơi để đo độ khó
            int currentScore = stats.calculateDifficultyScore();

            if (currentScore > bestScore) {
                bestScore = currentScore;
                bestBoard = board;
            }

            if (currentScore >= minScore && currentScore <= maxScore) {
                board.prepareGame(); // THÊM DÒNG NÀY: Khóa đề bài và dọn sạch Stack Undo
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

// ====================================================================
// KHỐI THUẬT TOÁN AC-3 VÀ BITWISE CẮT TỈA TỐI HƯU HIỆU NĂNG 
// ====================================================================
namespace {
    inline int countBits(uint16_t n) {
        int count = 0;
        while (n) { n &= (n - 1); count++; }
        return count;
    }

    // 1. THUẬT TOÁN AC-3 (Arc Consistency 3) 
    // Dùng Hàng đợi (Queue) để lan truyền ràng buộc dây chuyền
    bool propagateAC3(uint16_t domains[81], std::vector<int>& queue) {
        while (!queue.empty()) {
            int curr = queue.back();
            queue.pop_back();
            
            uint16_t val = domains[curr];
            int r = curr / 9;
            int c = curr % 9;
            int b = (r / 3) * 3 + (c / 3);

            // Thu thập các ô bị ảnh hưởng (Peers)
            int peers[24];
            int peerCount = 0;
            for (int i = 0; i < 9; ++i) {
                int pr = r, pc = i;
                if (pc != c) peers[peerCount++] = pr * 9 + pc; // Cùng hàng
                pr = i, pc = c;
                if (pr != r) peers[peerCount++] = pr * 9 + pc; // Cùng cột
                pr = (b / 3) * 3 + (i / 3);
                pc = (b % 3) * 3 + (i % 3);
                if (pr != r && pc != c) peers[peerCount++] = pr * 9 + pc; // Cùng khối 3x3
            }

            // Tiến hành quét AC-3
            for (int i = 0; i < peerCount; ++i) {
                int peer = peers[i];
                if (domains[peer] & val) { // Nếu ô hàng xóm chứa giá trị này trong tập hợp
                    domains[peer] &= ~val; // Xóa giá trị đó đi
                    
                    if (domains[peer] == 0) return false; // Ngõ cụt -> Suy luận sai
                    
                    // MA THUẬT CỦA AC-3 NẰM Ở ĐÂY:
                    // Nếu sau khi xóa, ô đó chỉ còn đúng 1 lựa chọn -> Bơm lại vào Queue để lan truyền tiếp!
                    if (countBits(domains[peer]) == 1) {
                        queue.push_back(peer); 
                    }
                }
            }
        }
        return true;
    }

    // 2. BACKTRACKING ĐẾM NGHIỆM KẾT HỢP MRV VÀ AC-3
    void fastCountSolutionsAC3(uint16_t domains[81], int& count) {
        if (count >= 2) return; 

        // Heuristic MRV: Tìm ô còn ít lựa chọn nhất
        int bestCell = -1;
        int minOptions = 10;
        for (int i = 0; i < 81; ++i) {
            int opts = countBits(domains[i]);
            if (opts > 1 && opts < minOptions) {
                minOptions = opts;
                bestCell = i;
                if (opts == 2) break; // 2 lựa chọn là tốt nhất có thể, ngắt vòng lặp tìm kiếm
            }
        }

        // Bảng đã được giải kín
        if (bestCell == -1) { 
            count++;
            return;
        }

        uint16_t available = domains[bestCell];
        for (int num = 1; num <= 9; ++num) {
            int bit = 1 << (num - 1);
            if (available & bit) {
                // Tạo một bản sao vùng nhớ để chạy AC-3 giả lập (Look-ahead)
                uint16_t nextDomains[81];
                std::copy(domains, domains + 81, nextDomains);
                
                nextDomains[bestCell] = bit;
                std::vector<int> q;
                q.push_back(bestCell);
                
                // Nếu AC-3 lan truyền thành công không gặp ngõ cụt thì mới đi sâu đệ quy
                if (propagateAC3(nextDomains, q)) {
                    fastCountSolutionsAC3(nextDomains, count);
                }
                if (count >= 2) return; // Thoát sớm
            }
        }
    }
}

int SudokuGenerator::countSolutions(Board board) {
    uint16_t domains[81];
    for (int i = 0; i < 81; ++i) domains[i] = 0x1FF; // 0x1FF = 9 bits 1 = {1,2,3,4,5,6,7,8,9}

    std::vector<int> queue;
    
    // Đọc bảng hiện tại, quy đổi sang Bitmask và cho hết các ô đã điền vào hàng đợi AC-3
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (board.getCell(r, c).value.has_value()) {
                int idx = r * 9 + c;
                domains[idx] = 1 << (board.getCell(r, c).value.value() - 1);
                queue.push_back(idx);
            }
        }
    }

    // Nếu bảng hiện tại vốn đã sai ràng buộc, trả về 0 nghiệm
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
    // CÔNG THỨC: Điểm = Số ô trống (maxDepth) * 15 + Số lần đoán mò (Backtrack) * 8
    
    if (difficulty == "Easy") { 
        // Bắt buộc phải xóa ít nhất ~30 ô (30 * 15 = 450) mới được dừng
        minScore = 450;    
        maxScore = 600; 
    }
    else if (difficulty == "Medium") { 
        // Khoảng 42 - 48 ô trống, bắt đầu có 1-2 lần đoán mò
        minScore = 650;  
        maxScore = 800; 
    }
    else if (difficulty == "Hard") { 
        // Khoảng 50 - 55 ô trống, cần đoán mò nhiều hơn
        minScore = 850;  
        maxScore = 1100; 
    }
    else if (difficulty == "Expert") { 
        minScore = 1200; 
        maxScore = 2000; 
    }
    else if (difficulty == "Master") { 
        minScore = 2200; 
        maxScore = 5000; 
    }
    else if (difficulty == "Extreme") { 
        // Giới hạn siêu khó của Sudoku, xóa đến mức gần mất nghiệm duy nhất
        minScore = 5001; 
        maxScore = 999999; 
    }
    else { 
        // Mặc định là Easy nếu lỗi chuỗi
        minScore = 450;    
        maxScore = 600; 
    } 
}