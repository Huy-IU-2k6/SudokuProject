#include "PlayingState.h"
#include <iostream>

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
}

void PlayingState::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    mGameUI.handleEvent(event, window);
    mDifficultyBar.handleEvent(event, window);
}