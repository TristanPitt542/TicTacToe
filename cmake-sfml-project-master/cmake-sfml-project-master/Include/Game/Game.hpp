#pragma once

#include <SFML/Graphics.hpp>

class Game
{
private:
    // Window
    std::unique_ptr<sf::RenderWindow> window;
    sf::Vector2f windowSize = sf::Vector2f(600, 600);
    std::string WindowTitle = "TIC TAC TOE";
    int FPSLimit = 60;

    //GameState
    enum GameState {
        MENU,
        SINGLEPLAYER,
        MULTIPLAYER,
        HOST,
        CLIENT
    };
    enum GameState gameState = MENU;

public:

    Game() = default;

    void InitialiseWindow();

    void Run();

    void Menu();
    
    void Input();
};