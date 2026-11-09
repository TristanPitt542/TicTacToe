/** @file PlaySettings.hpp */

#pragma once

#include "MenuScreen.hpp"
#include "Button.hpp"
#include "Text.hpp"
#include <GameSettings.hpp>

class GameSettings;

/** Menu for changing board dimensions and win length before a game starts. */
class PlaySettings : public MenuScreen {

private:
    // Board width controls.
    /// Text display showing current board width
    Text* m_boardWidth = nullptr;
    /// Button to decrease board width
    Button* m_boardWidthMinus = nullptr;
    /// Button to increase board width
    Button* m_boardWidthPlus = nullptr;

    // Board height controls.
    /// Text display showing current board height
    Text* m_boardHeight = nullptr;
    /// Button to decrease board height
    Button* m_boardHeightMinus = nullptr;
    /// Button to increase board height
    Button* m_boardHeightPlus = nullptr;

    // Win length controls.
    /// Text display showing current win length requirement
    Text* m_winLength = nullptr;
    /// Button to decrease win length
    Button* m_winLengthMinus= nullptr;
    /// Button to increase win length
    Button* m_winLengthPlus = nullptr;

    // Actions.
    /// Button to start the game with current settings
    Button* m_play = nullptr;
    /// Button to return to previous menu without changing settings
    Button* m_back = nullptr;


public:
    /** Creates the settings controls. */
    void Init(const sf::Vector2u& windowSize, const sf::Font& font) override {
        m_elements.clear();
        float centerX = windowSize.x / 2.f;

        auto title = std::make_unique<Text>(sf::Vector2f(centerX, 100.f), "Play Settings", font, 60.0f);
        m_elements.push_back(std::move(title));

        auto boardWidth = std::make_unique<Text>(sf::Vector2f(centerX, 250.f), "Board Width: " + std::to_string(static_cast<int>(GameSettings::GetBoardSize().x)), font);
        auto boardWidthMinus = std::make_unique<Button>(sf::Vector2f(centerX - (centerX / 2), 250.f), "-", font, sf::Vector2f(50, 50));
        auto boardWidthPlus = std::make_unique<Button>(sf::Vector2f(centerX + (centerX / 2), 250.f), "+", font, sf::Vector2f(50, 50));

        // Store non-owning pointers before transferring ownership to m_elements.
        m_boardWidth = boardWidth.get();
        m_boardWidthMinus = boardWidthMinus.get();
        m_boardWidthPlus = boardWidthPlus.get();

        m_elements.push_back(std::move(boardWidth));
        m_elements.push_back(std::move(boardWidthMinus));
        m_elements.push_back(std::move(boardWidthPlus));

        auto boardHeight = std::make_unique<Text>(sf::Vector2f(centerX, 350.f), "Board Height: " + std::to_string(static_cast<int>(GameSettings::GetBoardSize().y)), font);
        auto boardHeightMinus = std::make_unique<Button>(sf::Vector2f(centerX - (centerX / 2), 350.f), "-", font, sf::Vector2f(50, 50));
        auto boardHeightPlus = std::make_unique<Button>(sf::Vector2f(centerX + (centerX / 2), 350.f), "+", font, sf::Vector2f(50, 50));

        m_boardHeight = boardHeight.get();
        m_boardHeightMinus = boardHeightMinus.get();
        m_boardHeightPlus = boardHeightPlus.get();

        m_elements.push_back(std::move(boardHeight));
        m_elements.push_back(std::move(boardHeightMinus));
        m_elements.push_back(std::move(boardHeightPlus));

        auto winLength = std::make_unique<Text>(sf::Vector2f(centerX, 450.f), "Win Length: " + std::to_string(static_cast<int>(GameSettings::GetWinLength())), font);
        auto winLengthMinus = std::make_unique<Button>(sf::Vector2f(centerX - (centerX / 2), 450.f), "-", font, sf::Vector2f(50, 50));
        auto winLengthPlus = std::make_unique<Button>(sf::Vector2f(centerX + (centerX / 2), 450.f), "+", font, sf::Vector2f(50, 50));

        m_winLength = winLength.get();
        m_winLengthMinus = winLengthMinus.get();
        m_winLengthPlus = winLengthPlus.get();

        m_elements.push_back(std::move(winLength));
        m_elements.push_back(std::move(winLengthMinus));
        m_elements.push_back(std::move(winLengthPlus));

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

    /** Clears button state after input is handled. */
    void ResetClickState() {
        for (Button* button : { m_boardWidthMinus, m_boardWidthPlus, m_boardHeightMinus, m_boardHeightPlus, m_play, m_back }) {
            if (button) {
                button->ResetClickState();
            }
        }
    }

    /** Changes board dimensions and reduces win length when necessary. */
    void ChangeBoardSize(sf::Vector2i boardSizeDelta) {
        sf::Vector2i newBoardSize = GameSettings::GetBoardSize() + boardSizeDelta;

        // Keep both dimensions valid.
        if (newBoardSize.x < 3) newBoardSize.x = 3;
        if (newBoardSize.y < 3) newBoardSize.y = 3;

        GameSettings::SetBoardSize(newBoardSize);

        // A line cannot be longer than the smaller board dimension.
        int currentWinLength = static_cast<int>(GameSettings::GetWinLength());
        int maxAllowed = std::min(newBoardSize.x, newBoardSize.y);

        if (currentWinLength > maxAllowed) {
            GameSettings::SetWinLength(static_cast<float>(maxAllowed));
            if (m_winLength) {
                m_winLength->SetText("Win Length: " + std::to_string(maxAllowed));
            }
        }

        if (m_boardWidth)  m_boardWidth->SetText("Board Width: " + std::to_string(newBoardSize.x));
        if (m_boardHeight) m_boardHeight->SetText("Board Height: " + std::to_string(newBoardSize.y));
    }

    /** Changes win length within the valid range for the current board. */
    void ChangeWinLength(int winLengthDelta) {
        int currentWinLength = static_cast<int>(GameSettings::GetWinLength());
        int newWinLength = currentWinLength + winLengthDelta;

        sf::Vector2i boardSize = GameSettings::GetBoardSize();

        int maxAllowedWinLength = std::min(boardSize.x, boardSize.y);

        // Keep win length between the game minimum and board limit.
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

    // Button state queries.

    /**
     * @brief Checks if the board width decrease button was pressed
     * @return true if button was clicked this frame
     */
    bool IsWidthMinusPressed() const { return m_boardWidthMinus->IsClicked(); };

    /**
     * @brief Checks if the board width increase button was pressed
     * @return true if button was clicked this frame
     */
    bool IsWidthPlusPressed() const { return m_boardWidthPlus->IsClicked(); };

    /**
     * @brief Checks if the board height decrease button was pressed
     * @return true if button was clicked this frame
     */
    bool IsHeightMinusPressed() const { return m_boardHeightMinus->IsClicked(); };

    /**
     * @brief Checks if the board height increase button was pressed
     * @return true if button was clicked this frame
     */
    bool IsHeightPlusPressed() const { return m_boardHeightPlus->IsClicked(); };

    /**
     * @brief Checks if the win length decrease button was pressed
     * @return true if button was clicked this frame
     */
    bool IsWinLengthMinusPressed() const { return m_winLengthMinus->IsClicked(); };

    /**
     * @brief Checks if the win length increase button was pressed
     * @return true if button was clicked this frame
     */
    bool IsWinLengthPlusPressed() const { return m_winLengthPlus->IsClicked(); };

    /**
     * @brief Checks if the Play button was pressed
     * @return true if button was clicked this frame
     */
    bool IsPlayPressed() const { return m_play->IsClicked(); };

    /**
     * @brief Checks if the Back button was pressed
     * @return true if button was clicked this frame
     */
    bool IsBackPressed() const { return m_back->IsClicked(); };


};