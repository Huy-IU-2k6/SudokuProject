#include "Application.h"
#include <iostream>
#include <stdexcept>

int main() {
    try {
        Application app;
        app.run();
    } 
    catch (const std::exception& e) {
        // Nếu có lỗi, in ra màn hình Console
        std::cerr << "\n========================================\n";
        std::cerr << "[GAME CRASH]: " << e.what() << "\n";
        std::cerr << "========================================\n";
        
        // Giữ cửa sổ Console không bị tắt để bạn kịp đọc lỗi
        std::cout << "\nNhan Enter de thoat...";
        std::cin.get(); 
        return 1; // Trả về 1 báo hiệu chương trình kết thúc có lỗi
    }
    
    return 0;
}