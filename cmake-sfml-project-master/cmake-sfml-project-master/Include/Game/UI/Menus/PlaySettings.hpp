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
    
    Text* m_winLength = nullptr;
    Button* m_winLengthMinus= nullptr;
    Button* m_winLengthPlus = nullptr;

    Button* m_play = nullptr;

    Button* m_back = nullptr;


public:
    void Init(const sf::Vector2u& windowSize, const sf::Font& font) override {
        m_elements.clear();
        float centerX = windowSize.x / 2.f;

        // Title
        auto title = std::make_unique<Text>(sf::Vector2f(centerX, 100.f), "Play Settings", font, 60.0f);
        m_elements.push_back(std::move(title));

        // Board Width
        auto boardWidth = std::make_unique<Text>(sf::Vector2f(centerX, 250.f), "Board Width: " + std::to_string(static_cast<int>(GameSettings::GetBoardSize().x)), font);
        auto boardWidthMinus = std::make_unique<Button>(sf::Vector2f(centerX - (centerX / 2), 250.f), "-", font, sf::Vector2f(50, 50));
        auto boardWidthPlus = std::make_unique<Button>(sf::Vector2f(centerX + (centerX / 2), 250.f), "+", font, sf::Vector2f(50, 50));

        // CRITICAL: Capture the raw pointers directly from the unique_ptr before moving ownership
        m_boardWidth = boardWidth.get();
        m_boardWidthMinus = boardWidthMinus.get();
        m_boardWidthPlus = boardWidthPlus.get();

        m_elements.push_back(std::move(boardWidth));
        m_elements.push_back(std::move(boardWidthMinus));
        m_elements.push_back(std::move(boardWidthPlus));

        // Board Height
        auto boardHeight = std::make_unique<Text>(sf::Vector2f(centerX, 350.f), "Board Height: " + std::to_string(static_cast<int>(GameSettings::GetBoardSize().y)), font);
        auto boardHeightMinus = std::make_unique<Button>(sf::Vector2f(centerX - (centerX / 2), 350.f), "-", font, sf::Vector2f(50, 50));
        auto boardHeightPlus = std::make_unique<Button>(sf::Vector2f(centerX + (centerX / 2), 350.f), "+", font, sf::Vector2f(50, 50));

        m_boardHeight = boardHeight.get();
        m_boardHeightMinus = boardHeightMinus.get();
        m_boardHeightPlus = boardHeightPlus.get();

        m_elements.push_back(std::move(boardHeight));
        m_elements.push_back(std::move(boardHeightMinus));
        m_elements.push_back(std::move(boardHeightPlus));

        // Win Length
        auto winLength = std::make_unique<Text>(sf::Vector2f(centerX, 450.f), "Win Length: " + std::to_string(static_cast<int>(GameSettings::GetWinLength())), font);
        auto winLengthMinus = std::make_unique<Button>(sf::Vector2f(centerX - (centerX / 2), 450.f), "-", font, sf::Vector2f(50, 50));
        auto winLengthPlus = std::make_unique<Button>(sf::Vector2f(centerX + (centerX / 2), 450.f), "+", font, sf::Vector2f(50, 50));

        m_winLength = winLength.get();
        m_winLengthMinus = winLengthMinus.get();
        m_winLengthPlus = winLengthPlus.get();

        m_elements.push_back(std::move(winLength));
        m_elements.push_back(std::move(winLengthMinus));
        m_elements.push_back(std::move(winLengthPlus));

        // Play & Back
        auto play = std::make_unique<Button>(sf::Vector2f(centerX, 550), "Play", font);
        auto back = std::make_unique<Button>(sf::Vector2f(centerX, 650), "Back", font);

        m_play = play.get();
        m_back = back.get();

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

    void ChangeBoardSize(sf::Vector2i boardSizeDelta) {
        sf::Vector2i newBoardSize = GameSettings::GetBoardSize() + boardSizeDelta;

        // Prevent the board from getting too small (e.g., minimum 3x3)
        if (newBoardSize.x < 3) newBoardSize.x = 3;
        if (newBoardSize.y < 3) newBoardSize.y = 3;

        GameSettings::SetBoardSize(newBoardSize);

        // If the current win length is now greater than the new board dimensions, scale it down
        int currentWinLength = static_cast<int>(GameSettings::GetWinLength());
        int maxAllowed = std::max(newBoardSize.x, newBoardSize.y);

        if (currentWinLength > maxAllowed) {
            GameSettings::SetWinLength(static_cast<float>(maxAllowed));
            if (m_winLength) {
                m_winLength->SetText("Win Length: " + std::to_string(maxAllowed));
            }
        }

        // Update the UI text
        if (m_boardWidth)  m_boardWidth->SetText("Board Width: " + std::to_string(newBoardSize.x));
        if (m_boardHeight) m_boardHeight->SetText("Board Height: " + std::to_string(newBoardSize.y));
    }

    void ChangeWinLength(int winLengthDelta) {
        int currentWinLength = static_cast<int>(GameSettings::GetWinLength());
        int newWinLength = currentWinLength + winLengthDelta;

        // Get current board dimensions
        sf::Vector2i boardSize = GameSettings::GetBoardSize();

        // The physical limit for a straight line is the largest dimension of the board
        int maxAllowedWinLength = std::max(boardSize.x, boardSize.y);

        // Clamp the value between a reasonable minimum (like 3) and the board maximum
        if (newWinLength < 3) {
            newWinLength = 3;
        }
        else if (newWinLength > maxAllowedWinLength) {
            newWinLength = maxAllowedWinLength;
        }

        GameSettings::SetWinLength(static_cast<float>(newWinLength));

        if (m_winLength) {
            m_winLength->SetText("Win Length: " + std::to_string(newWinLength));
        }
    }

    bool IsWidthMinusPressed() const { return m_boardWidthMinus->IsClicked(); };
    bool IsWidthPlusPressed() const { return m_boardWidthPlus->IsClicked(); };

    bool IsHeightMinusPressed() const { return m_boardHeightMinus->IsClicked(); };
    bool IsHeightPlusPressed() const { return m_boardHeightPlus->IsClicked(); };

    bool IsWinLengthMinusPressed() const { return m_winLengthMinus->IsClicked(); };
    bool IsWinLengthPlusPressed() const { return m_winLengthPlus->IsClicked(); };

    bool IsPlayPressed() const { return m_play->IsClicked(); };

    bool IsBackPressed() const { return m_back->IsClicked(); };


};