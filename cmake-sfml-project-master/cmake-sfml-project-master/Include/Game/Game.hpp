/** @file Game.hpp */

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Board.hpp"
#include "MainMenu.hpp"
#include "PlaySettings.hpp"
#include "GameOver.hpp"
#include <map>

/** Coordinates the window, menus, board, and main game loop. */
class Game
{
public:
	enum class Turn {
		Player1,
		Player2,
	};

	Game();

	~Game();

	void InitialiseWindow();

	void Init();

	void Run();

	void SwitchTurn();

	void ProcessInput(Board& board);

	void Menu();

	void PlaySettingsMenu();

	void GameOverMenu();

	void Ingame();

	void ExitGame();

private:
	// Window and rendering.
	/// The main SFML render window
	std::unique_ptr<sf::RenderWindow> window;
	/// Window dimensions in pixels
	sf::Vector2f m_windowSize = sf::Vector2f(600, 600);
	/// Title displayed in the window bar
	std::string m_WindowTitle = "TIC TAC TOE";
	/// Frames per second limit
	int m_FPSLimit = 60;
	/// Background color of the game window
	sf::Color m_background = sf::Color(50, 50, 50);

	// Assets.
	/// Font loaded from file for rendering text (menus, score, etc.)
	sf::Font m_gameFont;

	// Audio.
	/// Sound buffer for piece placement effect
	sf::SoundBuffer m_placeBuffer;
	/// Sound buffer for game over effect
	sf::SoundBuffer m_gameOverBuffer;
	/// Flag indicating if place sound was successfully loaded
	bool m_placeSoundLoaded = false;
	/// Flag indicating if game over sound was successfully loaded
	bool m_gameOverSoundLoaded = false;
	/// Flag indicating if background music was successfully loaded
	bool m_musicLoaded = false;

	/// Sound effect player
	sf::Sound m_sound{m_placeBuffer};
	/// Background music player
	sf::Music m_music;

	// Game state.

	enum class GameState {
		MENU,
		SINGLEPLAYER,
		PLAYSETTINGS,
		GAMEOVER,
		EXIT
	};

	/// Whose turn it currently is (Player1 or Player2)
	Turn m_currentTurn;

	/// Current game state (menu, gameplay, etc.)
	GameState m_gameState;

	/// The winner of the current game (X, O, or TIE/EMPTY)
	CellState m_winner;

	/// Map of game states to their corresponding menu screens
	std::map<GameState, std::unique_ptr<MenuScreen>> m_menus;

	/// The game board
	Board m_board;

	/// Tracks if the left mouse button was pressed in the previous frame (used for click detection)
	bool m_wasLeftMousePressed = false;
};