#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Board.hpp"
#include "MainMenu.hpp"
#include <map>

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

    void Init();
    
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

    // Font
    sf::Font m_gameFont;

    // GameState
    enum class GameState {
        MENU,
        SINGLEPLAYER,
        MULTIPLAYER,
        HOST,
        CLIENT
    };

    GameState m_gameState;

    std::map<GameState, std::unique_ptr<MenuScreen>> m_menus;

    Board m_board;

    bool m_wasLeftMousePressed = false;
};