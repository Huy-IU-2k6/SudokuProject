#include "PlayingState.h"
#include <iostream>
#include <iomanip>
#include <sstream>

PlayingState::PlayingState(StateStack& stack, Context context)
    : State(stack, context)
    , mGridView(*context.board, *context.font)
    , mGameUI(*context.font)
    , mDifficultyBar(*context.font)
{
    // Dựng các thành phần giao diện
    mGameUI.buildUI();
    mDifficultyBar.buildUI();
    // 1. LẤY TỪ BALO RA: Ép thanh Bar hiển thị đúng chữ (Hard, Master...) mà người chơi đã chọn
    mDifficultyBar.setActiveDifficulty(*context.difficultyLevel);
    // Sắp xếp tọa độ
    mDifficultyBar.setPosition({50.f, 30.f});
    mGridView.setPosition({50.f, 80.f});

    // Callback khi bấm "New Game" ở màn chơi: 
    // KHÔNG xóa màn chơi, chỉ đẩy màn hình chọn độ khó lên đè lên trên!
    mGameUI.setNewGameCallback([this]() {
        requestStackPush("DifficultySelection");
    });
    // 2. Nếu người chơi đổi độ khó ngay TRONG LÚC CHƠI, ta cũng cập nhật ngược lại Balo
    mDifficultyBar.setCallback([context](const std::string& level) {
        std::cout << "Player swapped difficulty to: " << level << "\n";
        *context.difficultyLevel = level; 
    });
}

void PlayingState::draw() {
    sf::RenderWindow& window = *getContext().window;

    // 1. TẠO TẤM MÀN CHE KHUẤT CÁC MÀN HÌNH Ở DƯỚI ĐÁY STACK
    sf::RectangleShape background(sf::Vector2f(window.getSize()));
    background.setFillColor(sf::Color(250, 250, 250));
    window.draw(background);

    // 2. VẼ GIAO DIỆN CHƠI GAME
    window.draw(mGridView);
    window.draw(mGameUI);
    window.draw(mDifficultyBar);
}

void PlayingState::update(const sf::RenderWindow& window) {
    mGameUI.update(window);
    mDifficultyBar.update(window);
    // --- LOGIC CẬP NHẬT THỜI GIAN THỰC ---
    
    // 1. Lấy tổng số giây đã trôi qua kể từ lúc State này được tạo ra
    int elapsedSeconds = static_cast<int>(mTimer.getElapsedTime().asSeconds());
    
    // 2. Quy đổi ra Phút và Giây
    int minutes = elapsedSeconds / 60;
    int seconds = elapsedSeconds % 60;

    // 3. Ép định dạng chuỗi thành MM:SS (ví dụ: 09:05 thay vì 9:5)
    std::ostringstream timeStream;
    timeStream << std::setfill('0') << std::setw(2) << minutes << ":"
               << std::setfill('0') << std::setw(2) << seconds;

    // 4. Gửi chuỗi thời gian đã format sang cho UI cập nhật
    mGameUI.setTime(timeStream.str());
}

void PlayingState::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    mGameUI.handleEvent(event, window);
    mDifficultyBar.handleEvent(event, window);
}