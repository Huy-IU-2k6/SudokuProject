#ifndef DIFFICULTY_BAR_H
#define DIFFICULTY_BAR_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>

namespace GUI {

class DifficultyBar : public sf::Drawable, public sf::Transformable {
public:
    // Callback alias to trigger events when a difficulty is clicked
    using Callback = std::function<void(const std::string&)>;

    DifficultyBar(const sf::Font& font);
    
    void buildUI();
    void setCallback(Callback callback);
    // THÊM HÀM NÀY: Ép thanh UI hiển thị đúng độ khó được truyền vào
    void setActiveDifficulty(const std::string& level);
    
    void update(const sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    const sf::Font& mFont;
    sf::Text mTitleText;

    // Struct to hold individual difficulty options
    struct Option {
        sf::Text text;
        std::string name;
        bool isHovered;

        // SFML 3 requirement: Text must be initialized with a font
        Option(const sf::Font& font) : text(font), isHovered(false) {}
    };

    std::vector<Option> mOptions;
    int mSelectedIndex;
    Callback mCallback;

    // Color Palette matching the UI mockup
    static const sf::Color COLOR_TITLE;
    static const sf::Color COLOR_NORMAL;
    static const sf::Color COLOR_HOVER;
    static const sf::Color COLOR_SELECTED;
};

}

#endif