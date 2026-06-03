#pragma once

#include "MenuScreen.hpp"
#include "Button.hpp"
#include "Text.hpp"

class GameOver : public MenuScreen {
private:
    Button* m_again = nullptr;
    Button* m_back = nullptr;
    CellState m_winner = CellState::TIE;

public:
    void Init(const sf::Vector2u& windowSize, const sf::Font& font) override {
        m_elements.clear();

        float centerX = windowSize.x / 2.f;
        std::string winner;
        sf::Color textColor = sf::Color::White;

        if (m_winner == CellState::X)   { winner = "Player X Wins"; textColor = sf::Color::Magenta; }
        if (m_winner == CellState::O)   { winner = "Player O Wins"; textColor = sf::Color::Cyan; }
        if (m_winner == CellState::TIE) { winner = "It's a TIE"; }

        auto titleText = std::make_unique<Text>(sf::Vector2f(centerX, 100.f), "Game Over", font, 60.0f);
        auto winnerText = std::make_unique<Text>(sf::Vector2f(centerX, 200.f), winner, font, 30.0f, textColor);
        auto back = std::make_unique<Button>(sf::Vector2f(centerX - (centerX / 2), 350.f), "Back To Main", font);
        auto again = std::make_unique<Button>(sf::Vector2f(centerX + (centerX / 2), 350.f), "Play Again", font);

        m_elements.push_back(std::move(titleText));
        m_elements.push_back(std::move(winnerText));
        m_elements.push_back(std::move(again));
        m_elements.push_back(std::move(back));

        m_back = static_cast<Button*>(m_elements.back().get());
        m_again = static_cast<Button*>(m_elements[m_elements.size() - 2].get());
    }

    void Update(const sf::Event& event, const sf::RenderWindow& window) override {
        for (auto& element : m_elements) {
            element->Update(event, window);
        }
    }

    void ResetClickState() {
        for (Button* button : { m_again, m_back }) {
            if (button) {
                button->ResetClickState();
            }
        }
    }

    // const correctness for getters
    bool isAgainPressed() const { return m_again && m_again->IsClicked(); }
    bool isBackPressed() const { return m_back && m_back->IsClicked(); }

    void SetWinner(CellState winner) { m_winner = winner; }
};