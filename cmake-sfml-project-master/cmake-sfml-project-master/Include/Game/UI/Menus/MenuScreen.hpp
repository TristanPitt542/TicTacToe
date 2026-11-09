/** @file MenuScreen.hpp */

#pragma once

#include <SFML/Graphics.hpp>
#include "UIElement.hpp"
#include <vector>
#include <memory>

/** Owns and updates the UI elements belonging to a menu. */
class MenuScreen {
protected:
    /// Menu elements are owned by the screen.
    std::vector<std::unique_ptr<UIElement>> m_elements;

public:
    virtual ~MenuScreen() = default;

    /** Creates the menu's elements for the current window. */
    virtual void Init(const sf::Vector2u& windowSize, const sf::Font& font) = 0;

    /** Forwards the event to each element. */
    virtual void Update(const sf::Event& event, const sf::RenderWindow& window) {
        for (auto& element : m_elements) {
            element->Update(event, window);
        }
    }

    /** Draws elements in insertion order. */
    void Draw(sf::RenderTarget& target) const {
        for (const auto& element : m_elements) {
            element->Draw(target);
        }
    }
};