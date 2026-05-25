#ifndef STATE_H
#define STATE_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "../core/board.h"
#include <string>

class StateStack;

class State {
public:
    // Shared resources across all states to maintain clean OOP encapsulation
    struct Context {
        // ĐÃ SỬA: Thêm std::string& difficulty vào đây để đồng bộ với file .cpp
        Context(sf::RenderWindow& window, sf::Font& font, Board& board, std::string& difficulty);
        
        sf::RenderWindow* window;
        sf::Font* font;
        Board* board;
        std::string* difficultyLevel;
    };

    using Ptr = std::unique_ptr<State>;

    State(StateStack& stack, Context context);
    virtual ~State() = default;

    virtual void draw() = 0;
    virtual void update(const sf::RenderWindow& window) = 0;
    virtual void handleEvent(const sf::Event& event, const sf::RenderWindow& window) = 0;

protected:
    void requestStackPush(std::string stateID);
    void requestStackPop();
    void requestStackClear();
    Context getContext() const;

private:
    StateStack* mStack;
    Context mContext;
};

#endif