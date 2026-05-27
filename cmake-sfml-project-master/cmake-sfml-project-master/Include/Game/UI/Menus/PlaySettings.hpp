#pragma once

#include "MenuScreen.hpp"
#include "Button.hpp"
#include "Text.hpp"
#include <GameSettings.hpp>

class GameSettings;

class PlaySettings : public MenuScreen {
private:
    Text* m_boardWidth = nullptr;
    
    Button* m_boardWidthMinus = nullptr;
    Button* m_boardWidthPlus = nullptr;

    Text* m_boardHeight = nullptr;
    Button* m_boardHeightMinus = nullptr;
    Button* m_boardHeightPlus = nullptr;
    
    Button* m_play = nullptr;

    Button* m_back = nullptr;


public:
    void Init(const sf::Vector2u& windowSize, const sf::Font& font) override {
        float centerX = windowSize.x / 2.f;

        auto title = std::make_unique<Text>(sf::Vector2f(centerX, 100.f), "Play Settings",font, 60.0f);
        
        auto boardWidth = std::make_unique<Text>(sf::Vector2f(centerX, 250.f), "Board Width: " + std::to_string(static_cast<int>(GameSettings::GetBoardSize().x)), font);
        auto boardWidthMinus = std::make_unique<Button>(sf::Vector2f(centerX - (centerX / 2), 250.f), "-", font, sf::Vector2f(50, 50));
        auto boardWidthPlus = std::make_unique<Button>(sf::Vector2f(centerX + (centerX / 2), 250.f), "+", font, sf::Vector2f(50, 50));

        auto boardHeight = std::make_unique<Text>(sf::Vector2f(centerX, 350.f), "Board Height: " + std::to_string(static_cast<int>(GameSettings::GetBoardSize().y)), font);
        auto boardHeightMinus = std::make_unique<Button>(sf::Vector2f(centerX - (centerX / 2), 350.f), "-", font, sf::Vector2f(50, 50));
        auto boardHeightPlus = std::make_unique<Button>(sf::Vector2f(centerX + (centerX / 2), 350.f), "+", font, sf::Vector2f(50, 50));
        
        auto play = std::make_unique<Button>(sf::Vector2f(centerX, 450), "Play", font);

        auto back = std::make_unique<Button>(sf::Vector2f(centerX, 550), "Back", font);

        m_boardWidthMinus = boardWidthMinus.get();
        m_boardWidthPlus = boardWidthPlus.get();
        m_boardHeightMinus = boardHeightMinus.get();
        m_boardHeightPlus = boardHeightPlus.get();
        m_boardWidth = boardWidth.get();
        m_boardHeight = boardHeight.get();

        m_play = play.get();

        m_back = back.get();

        m_elements.push_back(std::move(title));
        m_elements.push_back(std::move(boardWidth));
        m_elements.push_back(std::move(boardWidthMinus));
        m_elements.push_back(std::move(boardWidthPlus));
        m_elements.push_back(std::move(boardHeight));
        m_elements.push_back(std::move(boardHeightMinus));
        m_elements.push_back(std::move(boardHeightPlus));

        m_elements.push_back(std::move(play));

        m_elements.push_back(std::move(back));
    }

    virtual void Update(const sf::Event& event, const sf::RenderWindow& window) {
        for (auto& element : m_elements) {
            element->Update(event, window);
        }
    }

    void ResetClickState() {
        for (Button* button : { m_boardWidthMinus, m_boardWidthPlus, m_boardHeightMinus, m_boardHeightPlus, m_play, m_back }) {
            if (button) {
                button->ResetClickState();
            }
        }
    }

    void ChangeBoardSize(sf::Vector2i boardSize) {
        static_cast<void>(GameSettings::SetBoardSize(GameSettings::GetBoardSize() + boardSize));
        m_boardWidth->SetText("Board Width: " + std::to_string(static_cast<int>(GameSettings::GetBoardSize().x)));
        m_boardHeight->SetText("Board Height: " + std::to_string(static_cast<int>(GameSettings::GetBoardSize().y)));
    }

    bool IsWidthMinusPressed() const { return m_boardWidthMinus->IsClicked(); };
    bool IsWidthPlusPressed() const { return m_boardWidthPlus->IsClicked(); };
    bool IsHeightMinusPressed() const { return m_boardHeightMinus->IsClicked(); };
    bool IsHeightPlusPressed() const { return m_boardHeightPlus->IsClicked(); };

    bool IsPlayPressed() const { return m_play->IsClicked(); };

    bool IsBackPressed() const { return m_back->IsClicked(); };


};