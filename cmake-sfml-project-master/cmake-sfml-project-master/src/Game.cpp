#include "Game.hpp"
#include <iostream>

Game::Game() : m_board(sf::Vector2i(10, 10), m_windowSize)
{
    InitialiseWindow();
    m_gameState = GameState::MENU;

    Init();
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

void Game::Init()
{
    // 1. Load the persistent font file first
    if (!m_gameFont.openFromFile("Assets/Fonts/arial.ttf")) {
        std::cout << "Failed to load engine font!" << std::endl;
    }

    // 2. Allocate your menu container and pass the stable font reference directly
    auto mainMenu = std::make_unique<MainMenu>();
    mainMenu->Init(window->getSize(), m_gameFont);

    // 3. Move the menu interface container into your map
    m_menus[GameState::MENU] = std::move(mainMenu);
}

void Game::Run()
{
    m_currentTurn = Turn::Player1;

    while (window->isOpen())
    {
        // Handle window events
        while (const std::optional event = window->pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window->close();
            }

            // Forward the active event to your Menu System while in the menu loop
            if (m_gameState == GameState::MENU) {
                auto it = m_menus.find(m_gameState);
                if (it != m_menus.end()) {
                    it->second->Update(*event, *window);
                }
            }
        }

        window->clear();

        if (m_gameState == GameState::MENU) {
            Menu(); // Checks actions and draws
        }
        else if (m_gameState == GameState::SINGLEPLAYER) {
            Ingame();
        }

        window->display();
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
            else if (winner == CellState::TIE)
            {
                std::cout << "Tie!\n";
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
    // Draw the active menu layout screen
    auto it = m_menus.find(m_gameState);
    if (it != m_menus.end()) {
        it->second->Draw(*window);
    }

    // Process state change targets safely
    MainMenu* mainMenu = static_cast<MainMenu*>(m_menus[GameState::MENU].get());
    if (mainMenu && mainMenu->IsPlayPressed()) {

        mainMenu->ResetClickState(); // Clear the button's internal state flag

        m_board.ResetBoard();
        m_gameState = GameState::SINGLEPLAYER;
    }
}

void Game::Ingame()
{
    // Input Processing
    ProcessInput(m_board);

    // Draw operations only (No window clear or display wrappers here!)
    m_board.Draw(*window);
}