#include "Application.h"
#include <iostream>
#include <stdexcept>

Application::Application()
    // 1. NỚI RỘNG CỬA SỔ RA 1000x700 để chứa UI
    : mWindow(sf::VideoMode({1000, 700}), "Sudoku Platform", sf::Style::Default)
    , mFont()
    , mBoard()
    , mGridView(mBoard, mFont)
    , mGameUI(mFont) // Khởi tạo bộ UI bên phải (nhưng chưa vẽ gì cả)
    , mDifficultyBar(mFont)
    , mMainMenu(mFont)
    , mCurrentState(State::MainMenu) // B
{
    mWindow.setFramerateLimit(60);
    
    // BƯỚC 1: BẮT BUỘC PHẢI TẢI FONT TRƯỚC
    // Nhớ điều chỉnh thành "../assets/fonts/arial.ttf" nếu file .exe nằm trong thư mục build/Debug nhé!
    if (!mFont.openFromFile("assets/fonts/arial.ttf")) {
        throw std::runtime_error("Loi nghiem trong: Khong the mo file font arial.ttf!");
    }

    // BƯỚC 2: SAU KHI FONT ĐÃ CÓ DỮ LIỆU, MỚI CHO PHÉP XÂY DỰNG NÚT BẤM
    mGameUI.buildUI();
    // 2. BUILD UI VÀ SET TỌA ĐỘ NGAY TRÊN BẢNG SUDOKU
    mDifficultyBar.buildUI();
    mMainMenu.buildUI({1000.f, 700.f});
    mDifficultyBar.setPosition({50.f, 30.f}); // Tọa độ X bằng bảng Sudoku, Y cao hơn một chút
    
    // Gán hành động khi click chọn độ khó
    mDifficultyBar.setCallback([](const std::string& level) {
        std::cout << "Player changed difficulty to: " << level << "\n";
    });
    // Bind Button Callbacks via Lambda functions
    mMainMenu.setStartCallback([this]() {
        mCurrentState = State::Playing; // Switch view to playing state
    });

    mMainMenu.setExitCallback([this]() {
        mWindow.close(); // Terminate application safely
    });

    // BƯỚC 3: Ép bảng Sudoku sang góc trái (Cách viền x=50, y=80)
    mGridView.setPosition({50.f, 80.f});
}

void Application::run() {
    while (mWindow.isOpen()) {
        processEvents();
        
        // Update components selectively according to the active scene state
        if (mCurrentState == State::MainMenu) {
            mMainMenu.update(mWindow);
        } else if (mCurrentState == State::Playing) {
            mGameUI.update(mWindow); 
            mDifficultyBar.update(mWindow);
        }

        render();
    }
}

void Application::processEvents() {
    while (const std::optional event = mWindow.pollEvent()) {
        
        // 1. Sự kiện tắt cửa sổ
        if (event->is<sf::Event::Closed>()) {
            mWindow.close();
        }
        
        // 2. THUẬT TOÁN KHI NGƯỜI DÙNG KÉO GIÃN MÀN HÌNH (RESIZE)
        if (const auto* resized = event->getIf<sf::Event::Resized>()) {
            // Tỷ lệ khung hình hiện tại của cửa sổ
            float windowRatio = static_cast<float>(resized->size.x) / static_cast<float>(resized->size.y);
            // Tỷ lệ thiết kế gốc (1000 / 700)
            float viewRatio = 1000.f / 700.f;

            float sizeX = 1.f;
            float sizeY = 1.f;
            float posX = 0.f;
            float posY = 0.f;

            // Kỹ thuật Letterboxing: Nếu màn hình bị kéo sai tỷ lệ, tạo viền đen/trắng bù vào
            if (windowRatio > viewRatio) {
                sizeX = viewRatio / windowRatio;
                posX = (1.f - sizeX) / 2.f;
            } else {
                sizeY = windowRatio / viewRatio;
                posY = (1.f - sizeY) / 2.f;
            }

            // Tạo một Camera cố định kích thước 1000x700
            sf::View view(sf::FloatRect({0.f, 0.f}, {1000.f, 700.f}));
            view.setViewport(sf::FloatRect({posX, posY}, {sizeX, sizeY}));
            
            mWindow.setView(view);
        }

       // Delegate events to the appropriate scene handler
        if (mCurrentState == State::MainMenu) {
            mMainMenu.handleEvent(*event, mWindow);
        } else if (mCurrentState == State::Playing) {
            mGameUI.handleEvent(*event, mWindow);
            mDifficultyBar.handleEvent(*event, mWindow);
        }
    }
}

void Application::render() {
    // Xóa màu nền đen mặc định của phần thừa (Letterbox)
    mWindow.clear(sf::Color(250, 250, 250)); // Nền màu xám nhạt

    // 1. Lưu lại Camera hiện tại (Camera có thể đã bị co giãn)
    sf::View currentView = mWindow.getView();

    // 2. Tạm thời trả Camera về full màn hình thật để vẽ một cái phông nền chà bá che đi viền đen
    mWindow.setView(mWindow.getDefaultView());
    sf::RectangleShape background(sf::Vector2f(mWindow.getSize()));
    background.setFillColor(sf::Color(250, 250, 250)); // Cùng màu xám nhạt
    mWindow.draw(background);

    // 3. Lắp lại Camera co giãn của Game vào
    mWindow.setView(currentView);

    // Vẽ Game như bình thường
    if (mCurrentState == State::MainMenu) {
        mWindow.draw(mMainMenu);
    } else if (mCurrentState == State::Playing) {
        mWindow.draw(mGridView);
        mWindow.draw(mGameUI);
        mWindow.draw(mDifficultyBar);
    }
    mWindow.display();
}