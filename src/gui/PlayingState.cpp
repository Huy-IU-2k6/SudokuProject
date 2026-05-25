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
    
    // Đồng bộ mức độ khó
    mDifficultyBar.setActiveDifficulty(*context.difficultyLevel);
    mDifficultyBar.setPosition({50.f, 30.f});
    mGridView.setPosition({50.f, 80.f});

    mGameUI.setNewGameCallback([this]() {
        requestStackPush("DifficultySelection");
    });
    
    mDifficultyBar.setCallback([context](const std::string& level) {
        std::cout << "Player swapped difficulty to: " << level << "\n";
        *context.difficultyLevel = level; 
    });

    // KHI BẤM PHÍM SỐ: Không clearSelection để giữ nguyên Highlight
    mGameUI.setNumpadCallback([this, context](int number) {
        int row = mGridView.getSelectedRow();
        int col = mGridView.getSelectedCol();

        if (row != -1 && col != -1) {
            std::cout << "Filling number " << number << " at [" << row << "][" << col << "]\n";
        } else {
            std::cout << "Please select a cell first!\n";
        }
    });

    // KHI BẤM CÁC NÚT HÀNH ĐỘNG KHÁC: Giữ nguyên Highlight
    mGameUI.setActionCallback([this, context](const std::string& actionName) {
        int row = mGridView.getSelectedRow();
        int col = mGridView.getSelectedCol();
        
        if (row != -1 && col != -1) {
            std::cout << "Executing action [" << actionName << "] at [" << row << "][" << col << "]\n";
        } else {
            std::cout << "Executing action [" << actionName << "] (No cell selected)\n";
        }
    });
}

void PlayingState::draw() {
    sf::RenderWindow& window = *getContext().window;

    // Tạo màn che khuất các màn hình ở dưới đáy stack
    sf::RectangleShape background(sf::Vector2f(window.getSize()));
    background.setFillColor(sf::Color(250, 250, 250));
    window.draw(background);

    window.draw(mGridView);
    window.draw(mGameUI);
    window.draw(mDifficultyBar);
}

void PlayingState::update(const sf::RenderWindow& window) {
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