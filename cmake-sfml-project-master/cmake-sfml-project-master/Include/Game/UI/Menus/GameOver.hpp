/** @file GameOver.hpp */

#pragma once

#include "MenuScreen.hpp"
#include "Button.hpp"
#include "Text.hpp"

/** Displays the result of a completed game and the available next actions. */
class GameOver : public MenuScreen {
private:
    /// Button to start another game
    Button* m_again = nullptr;
    /// Button to return to main menu
    Button* m_back = nullptr;
    /// The winner of the game (X, O, or TIE)
    CellState m_winner = CellState::TIE;

public:
    /** Creates the result text and action buttons. */
    void Init(const sf::Vector2u& windowSize, const sf::Font& font) override {
        m_elements.clear();

        float centerX = windowSize.x / 2.f;
        std::string winner;
        sf::Color textColor = sf::Color::White;

        // Match the result text color to the winning piece.
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

        // The menu owns the buttons; these pointers are only used for input checks.
        m_back = static_cast<Button*>(m_elements.back().get());
        m_again = static_cast<Button*>(m_elements[m_elements.size() - 2].get());
    }

    void Update(const sf::Event& event, const sf::RenderWindow& window) override {
        for (auto& element : m_elements) {
            element->Update(event, window);
        }
    }

    /** Clears button state after input is handled. */
    void ResetClickState() {
        for (Button* button : { m_again, m_back }) {
            if (button) {
                button->ResetClickState();
            }
        }
    }

    /** Returns whether Play Again was clicked. */
    bool isAgainPressed() const { return m_again && m_again->IsClicked(); }

    /** Returns whether Back to Main was clicked. */
    bool isBackPressed() const { return m_back && m_back->IsClicked(); }

    /** Sets the result shown by the screen. */
    void SetWinner(CellState winner) { m_winner = winner; }
};