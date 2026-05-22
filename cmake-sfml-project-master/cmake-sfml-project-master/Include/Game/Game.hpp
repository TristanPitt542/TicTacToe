#pragma once

#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include "Button.hpp"

class Game
{
public:

    //Turn
    enum class Turn {
        Player1,
        Player2,
    };

    Game();

    void InitialiseWindow();

    void Run();

    void SwitchTurn();

    void ProcessInput(Board& board);

    void Menu();

    void Ingame();

private:
    // Window
    std::unique_ptr<sf::RenderWindow> window;
    sf::Vector2f m_windowSize = sf::Vector2f(600, 600);
    std::string m_WindowTitle = "TIC TAC TOE";
    int m_FPSLimit = 60;

    Turn m_currentTurn;

    sf::Font fnt;

    //GameState
    enum class GameState {
        MENU,
        SINGLEPLAYER,
        MULTIPLAYER,
        HOST,
        CLIENT
    };

    GameState m_gameState;

    Board m_board;

    bool m_wasLeftMousePressed = false;
};