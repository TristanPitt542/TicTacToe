#pragma once

#include "MenuScreen.hpp"
#include "Button.hpp"
#include "Text.hpp"

class GameOver : public MenuScreen {
private:
    Button* m_again = nullptr;
    Button* m_back = nullptr;

public:
    void Init(const sf::Vector2u& windowSize, const sf::Font& font) override {
        float centerX = windowSize.x / 2.f;

        auto title = std::make_unique<Text>(sf::Vector2f(centerX, 100.f), "Game Over", font, 60.0f);
        auto winner = std::make_unique<Text>(sf::Vector2f(centerX, 200.f), "Player ", font);
        auto back = std::make_unique<Button>(sf::Vector2f(centerX - (centerX / 2), 350.f), "Back To Main", font);
        auto again = std::make_unique<Button>(sf::Vector2f(centerX + (centerX / 2), 350.f), "Play Again", font);

        m_again = again.get();
        m_back = back.get();

        m_elements.push_back(std::move(title));
        m_elements.push_back(std::move(again));
        m_elements.push_back(std::move(winner));
        m_elements.push_back(std::move(back));
    }

    virtual void Update(const sf::Event& event, const sf::RenderWindow& window) {
        for (auto& element : m_elements) {
            element->Update(event, window);
        }
    }

    void ResetClickState() {
        for (Button* button : { m_again, m_back}) {
            if (button) {
                button->ResetClickState();
            }
        }
    }

    // Custom helper to check if a state change was requested
    bool isAgainPressed() const { return m_again && m_again->IsClicked(); }
    bool isBackPressed() const { return m_back && m_back->IsClicked(); }
};