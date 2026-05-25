#include "MainMenuState.h"

MainMenuState::MainMenuState(StateStack& stack, Context context)
    : State(stack, context)
    , mMainMenu(*context.font) // Lấy font từ shared context
{
    // Lấy kích thước cửa sổ để tự động căn giữa Menu
    sf::Vector2f windowSize = sf::Vector2f(context.window->getSize());
    mMainMenu.buildUI(windowSize);

    // Khi bấm Start, đẩy màn hình chọn độ khó lên trên đỉnh Ngăn xếp
    mMainMenu.setStartCallback([this]() {
        requestStackPush("DifficultySelection");
    });

    // Khi bấm Exit, xóa bỏ màn hình hiện tại (Ngăn xếp rỗng -> Game tắt)
    mMainMenu.setExitCallback([this]() {
        requestStackPop();
    });
}

void MainMenuState::draw() {
    getContext().window->draw(mMainMenu);
}

void MainMenuState::update(const sf::RenderWindow& window) {
    mMainMenu.update(window);
}

void MainMenuState::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    mMainMenu.handleEvent(event, window);
}