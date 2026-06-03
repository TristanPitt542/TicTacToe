#pragma once
#include <SFML/Graphics.hpp>

class GameSettings
{
private:
	static inline sf::Vector2i m_boardSize{ 3, 3 };
	static inline float m_masterVolume;
	static inline int m_winLength = 3;
public:
	//getters
	static const sf::Vector2i& GetBoardSize() { return m_boardSize; };
	static const float& GetVolume() { return m_masterVolume; };
	static float GetWinLength() { return m_winLength; };

	//Setters
	static void SetBoardSize(const sf::Vector2i& boardSize) {
		m_boardSize = sf::Vector2i(std::clamp(boardSize.x, 3, INT_MAX), std::clamp(boardSize.y, 3, INT_MAX));
	}

	static void SetVolume(float volume) { m_masterVolume = volume; };

	static void SetWinLength(float winLength) {
		float minSize = std::min(m_boardSize.x, m_boardSize.y);
		m_winLength = std::clamp(winLength, 3.0f, minSize);
	}
};