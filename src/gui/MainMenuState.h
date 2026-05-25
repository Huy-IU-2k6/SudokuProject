#ifndef MAIN_MENU_STATE_H
#define MAIN_MENU_STATE_H

#include "gui/State.h"
#include "gui/MainMenu.h"

class MainMenuState : public State {
public:
    MainMenuState(StateStack& stack, Context context);

    void draw() override;
    void update(const sf::RenderWindow& window) override;
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;

private:
    GUI::MainMenu mMainMenu;
};

#endif