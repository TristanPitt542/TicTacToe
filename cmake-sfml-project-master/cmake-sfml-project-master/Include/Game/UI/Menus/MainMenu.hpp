#pragma once

#include "MenuScreen.hpp"
#include "Button.hpp"
#include "Text.hpp"

class MainMenu : public MenuScreen {
private:
    Button* m_singlePlayerButton = nullptr;
    Button* m_multiPlayerButton = nullptr;
    Button* m_optionsButton = nullptr;

public:
    void Init(const sf::Vector2u& windowSize, const sf::Font& font) override {
        m_elements.clear();
        float centerX = windowSize.x / 2.f;

        auto title = std::make_unique<Text>(sf::Vector2f(centerX, 100.f), "MAIN MENU", font, 60.0f);
        auto singlePlayer = std::make_unique<Button>(sf::Vector2f(centerX, 250.f), "Singleplayer", font);
        auto multiPlayer = std::make_unique<Button>(sf::Vector2f(centerX, 350.f), "Multiplay", font);
        auto options = std::make_unique<Button>(sf::Vector2f(centerX, 450.f), "Options", font);

        m_singlePlayerButton = singlePlayer.get();
        m_multiPlayerButton = multiPlayer.get();
        m_optionsButton = options.get();

        m_elements.push_back(std::move(title));
        m_elements.push_back(std::move(singlePlayer));
        m_elements.push_back(std::move(multiPlayer));
        m_elements.push_back(std::move(options));
    }

    virtual void Update(const sf::Event& event, const sf::RenderWindow& window) {
        for (auto& element : m_elements) {
            element->Update(event, window);
        }
    }

    void ResetClickState() {
        for (Button* button : { m_singlePlayerButton, m_multiPlayerButton, m_optionsButton }) {
            if (button) {
                button->ResetClickState();
            }
        }
    }

    // Custom helper to check if a state change was requested
    bool IsSinglePlayerPressed() const { return m_singlePlayerButton && m_singlePlayerButton->IsClicked(); }
    bool IsPlayPressed() const { return m_singlePlayerButton && m_singlePlayerButton->IsClicked(); }
    bool IsOptionsPressed() const { return m_optionsButton && m_optionsButton->IsClicked(); }
};