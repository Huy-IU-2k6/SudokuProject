#include "PlayingState.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include "generator/SudokuGenerator.h"

PlayingState::PlayingState(StateStack& stack, Context context)
    : State(stack, context)
    , mGridView(*context.board, *context.font)
    , mGameUI(*context.font)
    , mDifficultyBar(*context.font)
    , mLoadingText(*context.font)
{
    // Dựng các thành phần giao diện
    mGameUI.buildUI();
    mDifficultyBar.buildUI();
    
    // Đồng bộ mức độ khó
    mDifficultyBar.setActiveDifficulty(*context.difficultyLevel);
    mDifficultyBar.setPosition({50.f, 30.f});
    mGridView.setPosition({50.f, 80.f});

    mGameUI.setNewGameCallback([this]() {
        requestStackPush("DifficultySelection");
    });
    
    mDifficultyBar.setCallback([this, context](const std::string& level) {
        std::cout << "Player swapped difficulty to: " << level << "\n";
        *context.difficultyLevel = level; 

        // KÍCH HOẠT LẠI ĐA LUỒNG: Sinh ma trận mới ngay khi bấm nút đổi độ khó
        mFutureBoard = SudokuGenerator::generateAsync(level);
        mIsGenerating = true;

        // Xóa trạng thái ô đang chọn cũ để tránh lỗi hiển thị highlight
        mGridView.clearSelection();
    });

    // -------------------------------------------------------------
    // KHI BẤM PHÍM SỐ (1-9): LƯU SỐ VÀO MA TRẬN
    // -------------------------------------------------------------
    mGameUI.setNumpadCallback([this, context](int number) {
        int row = mGridView.getSelectedRow();
        int col = mGridView.getSelectedCol();

        if (row != -1 && col != -1) {
            // LỆNH CỐT LÕI: Ghi con số vào bộ nhớ của Bảng
            context.board->setCellValue(row, col, number);
        }
    });

    // KHI BẤM CÁC NÚT HÀNH ĐỘNG KHÁC: Giữ nguyên Highlight
    // KHI BẤM CÁC NÚT HÀNH ĐỘNG (Undo, Erase, Hint, Solution)
    mGameUI.setActionCallback([this, context](const std::string& actionName) {
        int row = mGridView.getSelectedRow();
        int col = mGridView.getSelectedCol();
        
        // 1. GLOBAL ACTION: Xử lý Undo (Không cần kiểm tra tọa độ ô)
        if (actionName == "Undo") {
            context.board->undo();
        }
        // 2. LOCAL ACTIONS: Xử lý các nút yêu cầu phải chọn ô trước (Erase, Hint...)
        else if (row != -1 && col != -1) {
            if (actionName == "Erase") {
                context.board->clearCell(row, col);
            }
            // TODO: Các chức năng Hint, Solution sẽ được thêm vào đây sau
        }
    });
    // SETUP LOADING TEXT
    mLoadingText.setFont(*context.font);
    mLoadingText.setString("Generating Puzzle...\nPlease wait...");
    mLoadingText.setCharacterSize(40);
    mLoadingText.setFillColor(sf::Color(80, 80, 80));
    mLoadingText.setPosition({200.f, 300.f}); // Đặt tạm ra giữa màn hình

    // KÍCH HOẠT ĐA LUỒNG TẠI ĐÂY:
    // Đẩy tác vụ tạo map xuống Background Thread ngay khi vào màn hình chơi
    mFutureBoard = SudokuGenerator::generateAsync(*context.difficultyLevel);
    mIsGenerating = true; 

}

void PlayingState::draw() {
    sf::RenderWindow& window = *getContext().window;

    // Tạo màn che khuất các màn hình ở dưới đáy stack
    sf::RectangleShape background(sf::Vector2f(window.getSize()));
    background.setFillColor(sf::Color(250, 250, 250));
    window.draw(background);

    if (mIsGenerating) {
        window.draw(mLoadingText);
    } else {
        window.draw(mGridView);
        window.draw(mGameUI);
        window.draw(mDifficultyBar);
    }
}

void PlayingState::update(const sf::RenderWindow& window) {
    if (mIsGenerating) {
        // KIỂM TRA LUỒNG (POLLING): Xem Background Thread đã chạy xong chưa?
        // Đợi 0 giây (Check non-blocking) để game không bị đơ
        if (mFutureBoard.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            
            // 1. NHẬN KẾT QUẢ: Lấy bảng Sudoku hoàn chỉnh từ Thread đẩy vào Bảng chính
            *getContext().board = mFutureBoard.get(); 

            // 2. MỞ KHÓA MÀN HÌNH: Tắt cờ Loading, reset đồng hồ bắt đầu tính giờ chơi
            mIsGenerating = false;
            mTimer.restart(); 
            
            std::cout << "Generation Complete!\n"; // Báo ra Console để bạn dễ theo dõi
        }
    } else {
        // CHỈ CẬP NHẬT GAME BÌNH THƯỜNG KHI ĐÃ LOAD XONG
        mGameUI.update(window);
        mDifficultyBar.update(window);
        
        // Logic đếm thời gian
        int elapsedSeconds = static_cast<int>(mTimer.getElapsedTime().asSeconds());
        int minutes = elapsedSeconds / 60;
        int seconds = elapsedSeconds % 60;

        std::ostringstream timeStream;
        timeStream << std::setfill('0') << std::setw(2) << minutes << ":"
                   << std::setfill('0') << std::setw(2) << seconds;

        mGameUI.setTime(timeStream.str());
    }
}

void PlayingState::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    // 1. Chuyển giao sự kiện click cho các UI
    mGameUI.handleEvent(event, window);
    mDifficultyBar.handleEvent(event, window);
    mGridView.handleEvent(event, window);

    // 2. RADAR DÒ KHOẢNG TRẮNG: Tự động tắt Highlight nếu click ra ngoài các khu vực chức năng
    if (const auto* mouseClick = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseClick->button == sf::Mouse::Button::Left) {
            sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
            sf::Vector2f mouseCoords = window.mapPixelToCoords(mousePixelPos);

            // Xác định tọa độ an toàn của Lưới Sudoku (từ x=50, y=80, rộng 540x540)
            bool isInsideGrid = sf::FloatRect({50.f, 80.f}, {540.f, 540.f}).contains(mouseCoords);
            
            // Xác định tọa độ an toàn của cụm Nút bấm UI bên phải
            bool isInsideUI = (mouseCoords.x > 640.f); 
            
            // Xác định tọa độ an toàn của thanh Chọn độ khó
            bool isInsideTopBar = (mouseCoords.y < 75.f);

            // Nếu không nằm trong bất kỳ vùng an toàn nào -> Xóa Highlight
            if (!isInsideGrid && !isInsideUI && !isInsideTopBar) {
                mGridView.clearSelection();
            }
        }
    }
}