#include "DifficultyBar.h"
#include <iostream>

namespace GUI {

// Define the colors based on the clean UI mockup
const sf::Color DifficultyBar::COLOR_TITLE    = sf::Color(150, 150, 150); // Gray for "Difficulty:"
const sf::Color DifficultyBar::COLOR_NORMAL   = sf::Color(130, 140, 150); // Grayish blue for unselected
const sf::Color DifficultyBar::COLOR_HOVER    = sf::Color(80, 80, 100);   // Darker gray on hover
const sf::Color DifficultyBar::COLOR_SELECTED = sf::Color(65, 105, 225);  // Royal blue for selected

DifficultyBar::DifficultyBar(const sf::Font& font) 
    : mFont(font)
    , mTitleText(font)
    , mSelectedIndex(0) // Default selected index is 0 (Easy)
    , mCallback(nullptr)
{
}

void DifficultyBar::buildUI() {
    // 1. Setup the static "Difficulty:" title
    mTitleText.setString("Difficulty:");
    mTitleText.setCharacterSize(20);
    mTitleText.setFillColor(COLOR_TITLE);
    mTitleText.setStyle(sf::Text::Bold);
    mTitleText.setPosition({0.f, 0.f});

    std::vector<std::string> levels = {"Easy", "Medium", "Hard", "Expert", "Master", "Extreme"};
    
    // Calculate starting X position right after the title text + some spacing
    float currentX = mTitleText.getLocalBounds().position.x + mTitleText.getLocalBounds().size.x + 15.f;

    // 2. Generate clickable text options dynamically
    for (size_t i = 0; i < levels.size(); ++i) {
        Option opt(mFont);
        opt.name = levels[i];
        
        opt.text.setString(opt.name);
        opt.text.setCharacterSize(20);
        opt.text.setPosition({currentX, 0.f});
        
        // Highlight the default selected item
        if (i == mSelectedIndex) {
            opt.text.setFillColor(COLOR_SELECTED);
            opt.text.setStyle(sf::Text::Bold);
        } else {
            opt.text.setFillColor(COLOR_NORMAL);
            opt.text.setStyle(sf::Text::Bold); // Keep them bold to match UI
        }

        // Advance X coordinate for the next option
        currentX += opt.text.getLocalBounds().size.x + 20.f; // 20.f is the spacing between words
        
        mOptions.push_back(std::move(opt));
    }
}

void DifficultyBar::setCallback(Callback callback) {
    mCallback = std::move(callback);
}

void DifficultyBar::update(const sf::RenderWindow& window) {
    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mouseCoords = window.mapPixelToCoords(mousePixelPos);

    for (size_t i = 0; i < mOptions.size(); ++i) {
        // Combine parent's transform with text's local transform to get true global bounding box
        sf::Transform combinedTransform = getTransform() * mOptions[i].text.getTransform();
        sf::FloatRect bounds = combinedTransform.transformRect(mOptions[i].text.getLocalBounds());

        // Expand the hitbox slightly to make clicking text easier
        bounds.position.y -= 5.f;
        bounds.size.y += 10.f;

        mOptions[i].isHovered = bounds.contains(mouseCoords);

        // Manage hover color states (ignore color change if it's the selected item)
        if (i == mSelectedIndex) {
            mOptions[i].text.setFillColor(COLOR_SELECTED);
        } else if (mOptions[i].isHovered) {
            mOptions[i].text.setFillColor(COLOR_HOVER);
        } else {
            mOptions[i].text.setFillColor(COLOR_NORMAL);
        }
    }
}

void DifficultyBar::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (const auto* mouseClick = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseClick->button == sf::Mouse::Button::Left) {
            
            for (size_t i = 0; i < mOptions.size(); ++i) {
                if (mOptions[i].isHovered) {
                    
                    // Update selection state
                    mSelectedIndex = i; 
                    
                    // Trigger callback
                    if (mCallback) {
                        mCallback(mOptions[i].name);
                    }
                    break;
                }
            }
        }
    }
}

void DifficultyBar::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    
    target.draw(mTitleText, states);
    for (const auto& opt : mOptions) {
        target.draw(opt.text, states);
    }
}
void DifficultyBar::setActiveDifficulty(const std::string& level) {
    for (size_t i = 0; i < mOptions.size(); ++i) {
        if (mOptions[i].name == level) {
            mSelectedIndex = i;
            mOptions[i].text.setFillColor(COLOR_SELECTED);
        } else {
            mOptions[i].text.setFillColor(COLOR_NORMAL);
        }
    }
}

}