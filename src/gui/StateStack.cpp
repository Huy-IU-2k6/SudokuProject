#include "StateStack.h"
#include <cassert>
#include <string>

// Implementation of Context struct
State::Context::Context(sf::RenderWindow& window, sf::Font& font, Board& board, std::string& difficulty)
    : window(&window), font(&font), board(&board), difficultyLevel(&difficulty) {}

// Implementation of State base class
State::State(StateStack& stack, Context context) : mStack(&stack), mContext(context) {}
void State::requestStackPush(std::string stateID) { mStack->pushState(stateID); }
void State::requestStackPop() { mStack->popState(); }
void State::requestStackClear() { mStack->clearStates(); }
State::Context State::getContext() const { return mContext; }

// Implementation of StateStack data structure
StateStack::StateStack(State::Context context) : mStack(), mPendingList(), mContext(context), mFactories() {}

State::Ptr StateStack::createState(std::string stateID) {
    auto found = mFactories.find(stateID);
    assert(found != mFactories.end() && "State ID not registered!");
    return found->second();
}

void StateStack::pushState(std::string stateID) {
    mPendingList.push_back({Action::Push, stateID});
}

void StateStack::popState() {
    mPendingList.push_back({Action::Pop, ""});
}

void StateStack::clearStates() {
    mPendingList.push_back({Action::Clear, ""});
}

bool StateStack::isEmpty() const {
    return mStack.empty();
}

void StateStack::update(const sf::RenderWindow& window) {
    // Only update the active screen sitting on the top of the stack (LIFO principle)
    if (!mStack.empty()) {
        mStack.back()->update(window);
    }
    applyPendingChanges();
}

void StateStack::draw() {
    // Advanced DSA Traversal: Draw from bottom to top so overlays render correctly
    for (auto& state : mStack) {
        state->draw();
    }
}

void StateStack::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (!mStack.empty()) {
        mStack.back()->handleEvent(event, window);
    }
    applyPendingChanges();
}

void StateStack::applyPendingChanges() {
    for (const auto& change : mPendingList) {
        switch (change.action) {
            case Action::Push:
                mStack.push_back(createState(change.stateID));
                break;
            case Action::Pop:
                mStack.pop_back();
                break;
            case Action::Clear:
                mStack.clear();
                break;
        }
    }
    mPendingList.clear();
}