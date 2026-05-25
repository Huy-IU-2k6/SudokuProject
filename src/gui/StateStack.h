#ifndef STATE_STACK_H
#define STATE_STACK_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include "State.h"

class StateStack {
public:
    enum class Action {
        Push,
        Pop,
        Clear
    };

    StateStack(State::Context context);

    // Factory method wrapper to register dynamic state allocation
    template <typename T>
    void registerState(std::string stateID);

    void update(const sf::RenderWindow& window);
    void draw();
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);

    void pushState(std::string stateID);
    void popState();
    void clearStates();

    bool isEmpty() const;

private:
    State::Ptr createState(std::string stateID);
    void applyPendingChanges();

private:
    struct PendingChange {
        Action action;
        std::string stateID;
    };

    // The core data structure: vector acting as a stack to allow bottom-to-top rendering traversal
    std::vector<State::Ptr> mStack;
    std::vector<PendingChange> mPendingList;

    State::Context mContext;
    std::map<std::string, std::function<State::Ptr()>> mFactories;
};

template <typename T>
void StateStack::registerState(std::string stateID) {
    mFactories[stateID] = [this]() {
        return std::make_unique<T>(*this, mContext);
    };
}

#endif