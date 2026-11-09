/** @file GameSettings.hpp */

#pragma once
#include <SFML/Graphics.hpp>

/** Stores the settings shared by the game and its menus. */
class GameSettings
{
private:
	/// Board dimensions (width, height) in cells. Default is 3x3. Minimum is 3x3.
	static inline sf::Vector2i m_boardSize{ 3, 3 };
	/// Master audio volume level (0.0 to 1.0 range recommended)
	static inline float m_masterVolume;
	/// Number of pieces in a row required to win. Default is 3, clamped to board dimensions.
	static inline int m_winLength = 3;

public:
	/** Returns the board dimensions in cells. */
	static const sf::Vector2i& GetBoardSize() { return m_boardSize; };

	/** Returns the configured master volume. */
	static const float& GetVolume() { return m_masterVolume; };

	/** Returns the number of matching cells required to win. */
	static float GetWinLength() { return m_winLength; };

	/** Sets the board dimensions. Each dimension is clamped to a minimum of 3. */
	static void SetBoardSize(const sf::Vector2i& boardSize) {
		m_boardSize = sf::Vector2i(std::clamp(boardSize.x, 3, INT_MAX), std::clamp(boardSize.y, 3, INT_MAX));
	}

	/** Sets the master volume. */
	static void SetVolume(float volume) { m_masterVolume = volume; };

	/** Sets the win length, constrained by the current board dimensions. */
	static void SetWinLength(float winLength) {
		float minSize = std::min(m_boardSize.x, m_boardSize.y);
		m_winLength = std::clamp(winLength, 3.0f, minSize);
	}
};