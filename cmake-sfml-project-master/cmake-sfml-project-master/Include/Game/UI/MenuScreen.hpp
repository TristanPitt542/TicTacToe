#pragma once

#include <SFML/Graphics.hpp>
#include "UIElement.hpp"
#include <vector>
#include <memory>

class MenuScreen {
protected:
    std::vector<std::unique_ptr<UIElement>> m_elements;

public:
    virtual ~MenuScreen() = default;
    virtual void Init(const sf::Vector2u& windowSize, const sf::Font& font) = 0;

    virtual void Update(const sf::Event& event, const sf::RenderWindow& window) {
        for (auto& element : m_elements) {
            element->Update(event, window);
        }
    }

    void Draw(sf::RenderTarget& target) const {
        for (const auto& element : m_elements) {
            element->Draw(target);
        }
    }
};