#include "Game.hpp"
#include "Board.hpp"
#include <iostream>

Game::Game() : m_board(sf::Vector2i(3, 4), m_windowSize)
{
    InitialiseWindow();
    m_gameState = GameState::SINGLEPLAYER;
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
    while (window->isOpen()) {

        switch (m_gameState)
        {
        case Game::GameState::MENU:
            Menu();
            break;
        case Game::GameState::SINGLEPLAYER:
            // Input
            ProcessInput(m_board);
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


        // Clear
        window->clear();

        // Draw
        m_board.Draw(*window);

        // Display
        window->display();
    }
}

void Game::SwitchTurn() {
    // If it's X, make it O. Otherwise, make it X.
    m_currentTurn = (m_currentTurn == Turn::Player1) ? Turn::Player2 : Turn::Player1;
}

void Game::ProcessInput(Board& board)
{
    std::optional<sf::Event> event = window->pollEvent();

    while (event.has_value())
    {
        // Handle window close
        if (event->is<sf::Event::Closed>())
        {
            window->close();
        }

        // Handle mouse clicks
        if (const auto* mouseClick = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseClick->button == sf::Mouse::Button::Left)
            {
                CellState pieceToPlace = (m_currentTurn == Turn::Player1)
                    ? CellState::X
                    : CellState::O;

                bool moveSuccessful = board.HandleMouseClick(mouseClick->position, pieceToPlace);

                if (moveSuccessful)
                {
                    // Check for a winner IMMEDIATELY after a valid move
                    CellState winner = board.CheckWinCondition();

                    if (winner == CellState::X)
                    {
                        std::cout << "Player 1 (X) Wins!\n";
                        m_gameState = GameState::MENU; // Reset state or head back to menu
                    }
                    else if (winner == CellState::O)
                    {
                        std::cout << "Player 2 (O) Wins!\n";
                        m_gameState = GameState::MENU;
                    }
                    else
                    {
                        // No winner yet, proceed to next turn safely
                        SwitchTurn();
                    }
                }
            }
        }
        
        // Pull next event from OS queue
        event = window->pollEvent();
    }
}

void Game::Menu()
{
    sf::Font fnt;
    if (!fnt.openFromFile("Assets/Fonts/arial.ttf"))
    {
        std::cout << "Failed to load font from: " << std::filesystem::current_path() << "/Assets/Fonts/arial.ttf\n";
    }

    sf::Text text = sf::Text(fnt,"Menu");
}