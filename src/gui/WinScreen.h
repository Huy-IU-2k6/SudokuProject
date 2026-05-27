#ifndef WIN_SCREEN_H
#define WIN_SCREEN_H

#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>
#include "Button.h"

namespace GUI {

class WinScreen : public sf::Drawable {
public:
    WinScreen(const sf::Font& font);

    void update(const sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);

    void setNewGameCallback(std::function<void()> callback);

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void centerTextOrigin(sf::Text& text);

private:
    sf::RectangleShape mOverlay;
    sf::RectangleShape mPopupBox;
    sf::Text mTitle;
    sf::Text mMessage;
    
    std::unique_ptr<Button> mNewGameBtn;
    std::function<void()> mNewGameCallback;
};

}

#endif