#ifndef PLAYING_STATE_H
#define PLAYING_STATE_H

#include "gui/State.h"
#include "gui/GameUI.h"
#include "gui/DifficultyBar.h"
#include "gui/SudokuGrid.h"

class PlayingState : public State {
public:
    PlayingState(StateStack& stack, Context context);

    void draw() override;
    void update(const sf::RenderWindow& window) override;
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;

private:
    SudokuGrid mGridView;
    GUI::GameUI mGameUI;
    GUI::DifficultyBar mDifficultyBar;
};

#endif