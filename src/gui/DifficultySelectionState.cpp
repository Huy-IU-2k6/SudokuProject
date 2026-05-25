#include "DifficultySelectionState.h"
#include <iostream>

DifficultySelectionState::DifficultySelectionState(StateStack& stack, Context context)
    : State(stack, context)
    , mDifficultyMenu(*context.font)
{
    sf::Vector2f windowSize = sf::Vector2f(context.window->getSize());
    mDifficultyMenu.buildUI(windowSize);
    
    mDifficultyMenu.setCancelText("Go Back");

    // Khi bấm Go Back, gỡ bỏ màn hình này khỏi Ngăn xếp, màn hình cũ sẽ lộ ra
    mDifficultyMenu.setCancelCallback([this]() {
        requestStackPop(); 
    });

    // Khi chọn độ khó thành công
    mDifficultyMenu.setDifficultyCallback([this, context](const std::string& level) {
        std::cout << "Starting game with difficulty: " << level << "\n";
        // BỎ VÀO BALO: Lưu độ khó vừa chọn vào bộ nhớ chung
        *context.difficultyLevel = level;
        
        // TODO: context.board->generateNewPuzzle(level);

        // THUẬT TOÁN BẢO VỆ BỘ NHỚ: Xóa toàn bộ stack và nạp lại [MainMenu -> Playing] 
        // Điều này ngăn chặn việc người chơi bấm New Game liên tục làm tràn RAM
        requestStackClear();
        requestStackPush("MainMenu");
        requestStackPush("Playing");
    });
}

void DifficultySelectionState::draw() {
    sf::RenderWindow& window = *getContext().window;

    // 1. TẠO MỘT TẤM MÀN CHE TOÀN BỘ MÀN HÌNH Ở DƯỚI
    sf::RectangleShape background(sf::Vector2f(window.getSize()));
    background.setFillColor(sf::Color(250, 250, 250)); // Màu xám nhạt đồng bộ với game
    window.draw(background);

    // 2. VẼ MENU ĐỘ KHÓ LÊN TRÊN TẤM MÀN ĐÓ
    window.draw(mDifficultyMenu);
}

void DifficultySelectionState::update(const sf::RenderWindow& window) {
    mDifficultyMenu.update(window);
}

void DifficultySelectionState::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    mDifficultyMenu.handleEvent(event, window);
}