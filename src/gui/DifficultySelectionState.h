#ifndef DIFFICULTY_SELECTION_STATE_H
#define DIFFICULTY_SELECTION_STATE_H

#include "gui/State.h"
#include "gui/DifficultyMenu.h"

class DifficultySelectionState : public State {
public:
    DifficultySelectionState(StateStack& stack, Context context);

    void draw() override;
    void update(const sf::RenderWindow& window) override;
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;

private:
    GUI::DifficultyMenu mDifficultyMenu;
};

#endif