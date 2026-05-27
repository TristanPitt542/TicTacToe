#include "Game.hpp"
#include <iostream>

Game::Game() : m_board(sf::Vector2i(static_cast<int>(GameSettings::GetBoardSize().x), static_cast<int>(GameSettings::GetBoardSize().y)), m_windowSize), m_sound(m_buffer)
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
    // Load the font file first
    if (!m_gameFont.openFromFile("Assets/Fonts/arial.ttf"))
    {
        std::cout << "Failed to load engine font!" << std::endl;
    }

    // Load the sounds from file
    if (!m_buffer.loadFromFile("Assets/Sounds/thump.mp3"))
    {
        std::cout << "Failed to load engine sound!" << std::endl;
    }
    m_sound.setVolume(2);

    if (!m_music.openFromFile("Assets/Sounds/music.mp3"))
    {
        std::cout << "Failed to load engine music!" << std::endl;
    }
    m_music.setVolume(2);
    m_music.setLooping(true);
    m_music.play();

    // Allocate your menu container and pass the stable font reference directly
    auto mainMenu = std::make_unique<MainMenu>();
    mainMenu->Init(window->getSize(), m_gameFont);

    auto playSettings = std::make_unique<PlaySettings>();
    playSettings->Init(window->getSize(), m_gameFont);

    // Move the menu interface container into your map
    m_menus[GameState::MENU] = std::move(mainMenu);
    m_menus[GameState::PLAYSETTINGS] = std::move(playSettings);
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
            auto it = m_menus.find(m_gameState);
            if (it != m_menus.end()) {
                it->second->Update(*event, *window);
            }
        }

        window->clear(m_background);
        switch (m_gameState)
        {
        case Game::GameState::MENU:
            Menu();
            break;
        case Game::GameState::SINGLEPLAYER:
            Ingame();
            break;
        case Game::GameState::PLAYSETTINGS:
            PlaySettingsMenu();
            break;
        case Game::GameState::MULTIPLAYER:
            Ingame();
            break;
        case Game::GameState::HOST:
            break;
        case Game::GameState::CLIENT:
            break;
        default:
            break;
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
            m_sound.play();

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

        mainMenu->ResetClickState();

        m_gameState = GameState::PLAYSETTINGS;
    }
}

void Game::PlaySettingsMenu()
{
    // Draw the active menu layout screen
    auto it = m_menus.find(m_gameState);
    if (it != m_menus.end()) {
        it->second->Draw(*window);
    }

    // Process state change targets safely
    PlaySettings* playSettings = static_cast<PlaySettings*>(m_menus[GameState::PLAYSETTINGS].get());
    if (playSettings && playSettings->IsBackPressed()) {

        playSettings->ResetClickState();

        m_gameState = GameState::MENU;
    }
    else if (playSettings && playSettings->IsWidthMinusPressed()) {

        playSettings->ResetClickState();
       
        playSettings->ChangeBoardSize({ -1,0 });
    }
    else if (playSettings && playSettings->IsWidthPlusPressed()) {

        playSettings->ResetClickState();

        playSettings->ChangeBoardSize({ 1,0 });
    }
    else if (playSettings && playSettings->IsHeightMinusPressed()) {

        playSettings->ResetClickState();

        playSettings->ChangeBoardSize({ 0,-1 });
    }
    else if (playSettings && playSettings->IsHeightPlusPressed()) {

        playSettings->ResetClickState();

        playSettings->ChangeBoardSize({ 0,1 });
    }
    else if (playSettings && playSettings->IsPlayPressed()) {

        playSettings->ResetClickState();

        m_board.ResetBoard(sf::Vector2i(static_cast<int>(GameSettings::GetBoardSize().x), static_cast<int>(GameSettings::GetBoardSize().y)),m_windowSize);
        
        m_gameState = GameState::SINGLEPLAYER;
    }

}

void Game::Ingame()
{
    // Input Processing
    ProcessInput(m_board);

    // Render
    m_board.Draw(*window);
}