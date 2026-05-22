#include "Game.hpp"
#include <iostream>

Game::Game() : m_board(sf::Vector2i(3, 4), m_windowSize)
{
    InitialiseWindow();
    m_gameState = GameState::MENU;

    //Load Font
    if (!fnt.openFromFile("Assets/Fonts/arial.ttf"))
    {
        std::cout << "Failed to load font from: ";
    }
}

void Game::InitialiseWindow()
{
    window = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(sf::Vector2u(static_cast<unsigned int>(m_windowSize.x),
            static_cast<unsigned int>(m_windowSize.y))),
        m_WindowTitle
    );
    window->setFramerateLimit(m_FPSLimit);
}

void Game::Run()
{
    m_currentTurn = Turn::Player1;

    //Window Loop
    while (window->isOpen())
    {
        while (const std::optional event = window->pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window->close();
        }

        switch (m_gameState)
        {
        case Game::GameState::MENU:
            Menu();
            break;
        case Game::GameState::SINGLEPLAYER:
            Ingame();
            break;
        case Game::GameState::MULTIPLAYER:
            break;
        case Game::GameState::HOST:
            break;
        case Game::GameState::CLIENT:
            break;
        default:
            break;
        }
    }
}

void Game::SwitchTurn() {
    // If it's X, make it O. Otherwise, make it X.
    m_currentTurn = (m_currentTurn == Turn::Player1) ? Turn::Player2 : Turn::Player1;
}

void Game::ProcessInput(Board& board)
{
    // Get current mouse state
    bool isLeftMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

    // Detect release (was pressed last frame, now not pressed)
    if (m_wasLeftMousePressed && !isLeftMousePressed)
    {
        // Get mouse position relative to window
        sf::Vector2i mousePos = sf::Mouse::getPosition(*window);

        // Determine which piece to place
        CellState pieceToPlace = (m_currentTurn == Turn::Player1)
            ? CellState::X
            : CellState::O;

        // Handle the click
        bool moveSuccessful = board.HandleMouseClick(mousePos, pieceToPlace);

        if (moveSuccessful)
        {
            // Check for winner immediately
            CellState winner = board.CheckWinCondition();

            if (winner == CellState::X)
            {
                std::cout << "Player 1 (X) Wins!\n";
                m_gameState = GameState::MENU;
            }
            else if (winner == CellState::O)
            {
                std::cout << "Player 2 (O) Wins!\n";
                m_gameState = GameState::MENU;
            }
            else
            {
                SwitchTurn();
            }
        }
    }

    // Update previous state
    m_wasLeftMousePressed = isLeftMousePressed;

    // Optional: handle window close via Escape key
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
    {
        m_gameState = GameState::MENU;
    }
}

void Game::Menu()
{
    window->clear();

    Button startButton({m_windowSize.x / 2.f, 200});

    // Draw title
    sf::Text menuText(fnt, "MENU", 30);
    menuText.setPosition({ m_windowSize.x / 2.f, 0 });
    menuText.setLineAlignment(sf::Text::LineAlignment::Center);
    window->draw(menuText);

    // Draw button
    startButton.RenderButton(*window);

    startButton.Update(*window);

    // Check click (release detection handled internally)
    if (!startButton.IsClicked() && m_wasLeftMousePressed)
    {
        m_board.ResetBoard();
        m_gameState = GameState::SINGLEPLAYER;
    }

    m_wasLeftMousePressed = startButton.IsClicked();
    window->display();
}

void Game::Ingame()
{
    // Input
    ProcessInput(m_board);

    // Clear
    window->clear();

    // Draw
    m_board.Draw(*window);

    // Display
    window->display();
}