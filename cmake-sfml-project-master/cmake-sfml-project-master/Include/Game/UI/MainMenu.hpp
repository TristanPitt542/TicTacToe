#pragma once

#include "MenuScreen.hpp"
#include "Button.hpp"
#include "Text.hpp"

class MainMenu : public MenuScreen {
private:
    Button* m_playButton = nullptr;
    Button* m_optionsButton = nullptr;

public:
    void Init(const sf::Vector2u& windowSize, const sf::Font& font) override {
        float centerX = windowSize.x / 2.f;

        auto title = std::make_unique<Text>(sf::Vector2f(centerX, 100.f), "MAIN MENU", font);
        auto play = std::make_unique<Button>(sf::Vector2f(centerX, 250.f), "Play", font);
        auto options = std::make_unique<Button>(sf::Vector2f(centerX, 350.f), "Options", font);

        m_playButton = play.get();
        m_optionsButton = options.get();

        m_elements.push_back(std::move(title));
        m_elements.push_back(std::move(play));
        m_elements.push_back(std::move(options));
    }

    virtual void Update(const sf::Event& event, const sf::RenderWindow& window) {
        for (auto& element : m_elements) {
            element->Update(event, window);
        }
    }

    void ResetClickState() {
        if (m_playButton) {
            m_playButton->ResetClickState();
        }
    }

    // Custom helper to check if a state change was requested
    bool IsPlayPressed() const { return m_playButton && m_playButton->IsClicked(); }
    bool IsOptionsPressed() const { return m_optionsButton && m_optionsButton->IsClicked(); }
};