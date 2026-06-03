#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Board.hpp"
#include "MainMenu.hpp"
#include "PlaySettings.hpp"
#include "GameOver.hpp"
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

    void PlaySettingsMenu();

    void GameOverMenu();

    void Ingame();

private:
    // Window
    std::unique_ptr<sf::RenderWindow> window;
    sf::Vector2f m_windowSize = sf::Vector2f(600, 600);
    std::string m_WindowTitle = "TIC TAC TOE";
    int m_FPSLimit = 60;
    sf::Color m_background = sf::Color(50, 50, 50);

    // Font
    sf::Font m_gameFont;

    // Sound
    sf::SoundBuffer m_placeBuffer;
    sf::SoundBuffer m_gameOverBuffer;

    sf::Sound m_sound;
    sf::Music m_music;


    // GameState
    enum class GameState {
        MENU,
        SINGLEPLAYER,
        PLAYSETTINGS,
        MULTIPLAYER,
        GAMEOVER,
        HOST,
        CLIENT
    };

    Turn m_currentTurn;

    GameState m_gameState;

    CellState m_winner;

    std::map<GameState, std::unique_ptr<MenuScreen>> m_menus;

    Board m_board;

    bool m_wasLeftMousePressed = false;
};