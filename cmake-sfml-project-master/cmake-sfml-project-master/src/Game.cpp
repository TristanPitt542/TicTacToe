/** @file Game.cpp */

#include "Game.hpp"
#include <iostream>

Game::Game() : m_board(GameSettings::GetBoardSize(), m_windowSize), m_sound(m_placeBuffer)
{
	InitialiseWindow();
	m_gameState = GameState::MENU;

	Init();
}

Game::~Game()
{
	m_sound.stop();
	m_music.stop();
	window->close();
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
    // Load resources before constructing menus, which keep references to the font.
    if (!m_gameFont.openFromFile("Assets/Fonts/arial.ttf"))
    {
        std::cout << "Failed to load engine font!" << std::endl;
    }

    m_placeSoundLoaded = m_placeBuffer.loadFromFile("Assets/Sounds/thump.mp3")
        && m_placeBuffer.getChannelCount() > 0
        && m_placeBuffer.getSampleCount() > 0;
    if (!m_placeSoundLoaded)
    {
        std::cout << "Failed to load engine sound!" << std::endl;
    }
    m_sound.setVolume(2);

    m_gameOverSoundLoaded = m_gameOverBuffer.loadFromFile("Assets/Sounds/gameover.mp3")
        && m_gameOverBuffer.getChannelCount() > 0
        && m_gameOverBuffer.getSampleCount() > 0;
    if (!m_gameOverSoundLoaded)
    {
        std::cout << "Failed to load GameOver sound!" << std::endl;
    }

    m_musicLoaded = m_music.openFromFile("Assets/Sounds/Music.mp3");
    if (!m_musicLoaded)
    {
        std::cout << "Failed to load engine music!" << std::endl;
    }
    else
    {
        if (m_music.getDuration().asSeconds() <= 0)
        {
            m_musicLoaded = false;
            std::cout << "Music file has no valid audio data!" << std::endl;
        }
    }

    if (m_placeSoundLoaded)
        m_sound.setBuffer(m_placeBuffer);

    if (m_musicLoaded)
    { 
        m_music.setVolume(2);
        m_music.setLooping(true);
        m_music.play();
    }

    auto mainMenu = std::make_unique<MainMenu>();
    mainMenu->Init(window->getSize(), m_gameFont);

    auto playSettings = std::make_unique<PlaySettings>();
    playSettings->Init(window->getSize(), m_gameFont);

    auto gameOver = std::make_unique<GameOver>();
    gameOver->Init(window->getSize(), m_gameFont);

    m_menus[GameState::MENU] = std::move(mainMenu);
    m_menus[GameState::PLAYSETTINGS] = std::move(playSettings);
    m_menus[GameState::GAMEOVER] = std::move(gameOver);
}

void Game::Run()
{
    m_currentTurn = Turn::Player1;

    while (window->isOpen())
    {
        while (const std::optional event = window->pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window->close();
            }

            auto it = m_menus.find(m_gameState);
            if (it != m_menus.end()) {
                it->second->Update(*event, *window);
            }
        }

        window->clear(m_background);
        switch (m_gameState)
        {
        case GameState::MENU:
            Menu();
            break;
        case GameState::SINGLEPLAYER:
            Ingame();
            break;
        case GameState::PLAYSETTINGS:
            PlaySettingsMenu();
            break;
        case GameState::MULTIPLAYER:
            Ingame();
            break;
        case GameState::GAMEOVER:
            GameOverMenu();
            break;
        case GameState::HOST:
            break;
        case GameState::CLIENT:
            break;
        case GameState::EXIT:
            ExitGame();
			break;
        default:
            Menu();
            break;
        }

        window->display();
    }
}

void Game::SwitchTurn() {
    m_currentTurn = (m_currentTurn == Turn::Player1) ? Turn::Player2 : Turn::Player1;
}

void Game::ProcessInput(Board& board)
{
    bool isLeftMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

    // Process on release so a held mouse button does not place multiple pieces.
    if (m_wasLeftMousePressed && !isLeftMousePressed)
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(*window);

        CellState pieceToPlace = (m_currentTurn == Turn::Player1)
            ? CellState::X
            : CellState::O;

        bool moveSuccessful = board.HandleMouseClick(mousePos, pieceToPlace);

        if (moveSuccessful)
        {
            if (m_placeSoundLoaded)
            {
                m_sound.setBuffer(m_placeBuffer);
                m_sound.play();
            }

            CellState winner = board.CheckWinCondition(GameSettings::GetWinLength());

            if (winner == CellState::X || winner == CellState::O || winner == CellState::TIE)
            {
                if (m_gameOverSoundLoaded)
                {
                    m_sound.setBuffer(m_gameOverBuffer);
                    m_sound.play();
                }

                m_winner = winner;
                m_gameState = GameState::GAMEOVER;

                auto* gameOver = static_cast<GameOver*>(m_menus[GameState::GAMEOVER].get());
                if (gameOver) {
                    gameOver->SetWinner(m_winner);
                    gameOver->Init(window->getSize(), m_gameFont);
                }
            }
            else
            {
                SwitchTurn();
            }
        }
    }

    m_wasLeftMousePressed = isLeftMousePressed;

    // Handle window close via Escape key
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

	if (mainMenu && mainMenu->IsExitPressed()) {
		mainMenu->ResetClickState();
		m_gameState = GameState::EXIT;
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

        m_board.ResetBoard(GameSettings::GetBoardSize(),m_windowSize);
        
        m_gameState = GameState::SINGLEPLAYER;
    }
    else if (playSettings && playSettings->IsWinLengthMinusPressed()) {
        
        playSettings->ResetClickState();

        playSettings->ChangeWinLength(-1);
    }
    else if (playSettings && playSettings->IsWinLengthPlusPressed()) {
        
        playSettings->ResetClickState();
        
        playSettings->ChangeWinLength(1);
    }

}

void Game::GameOverMenu()
{
    // Draw the active menu layout screen
    auto it = m_menus.find(m_gameState);
    if (it != m_menus.end()) {
        it->second->Draw(*window);
    }

    // Safely grab the screen to check for button clicks
    GameOver* gameOver = static_cast<GameOver*>(m_menus[GameState::GAMEOVER].get());
    if (!gameOver) return;

    if (gameOver->isAgainPressed()) {
        m_board.ResetBoard(GameSettings::GetBoardSize(), m_windowSize);
        gameOver->ResetClickState();
        m_gameState = GameState::SINGLEPLAYER;
    }
    else if (gameOver->isBackPressed()) {
        gameOver->ResetClickState();
        m_gameState = GameState::MENU;
    }
}

void Game::Ingame()
{
    // Input Processing
    ProcessInput(m_board);

    // Render
    m_board.Draw(*window);
}

void Game::ExitGame()
{
    m_sound.stop();
    m_music.stop();
    window->close();
}
