#pragma once
#include <SFML/Graphics.hpp>

enum class CellState { EMPTY, X, O , TIE};

class Board
{
private:
	sf::Vector2i m_boardSize;
	float m_cellSize;
	sf::Vector2f m_boardOffset;

	std::vector<CellState> m_board;

protected:
	// Colors
	sf::Color m_oColor = sf::Color::Cyan;
	sf::Color m_xColor = sf::Color::Magenta;

public:

	Board(sf::Vector2i boardSize, sf::Vector2f widnowSize);
	~Board();

	//Input
	bool HandleMouseClick(sf::Vector2i mousePos, CellState playerPiece);

	//Check Board
	CellState CheckWinCondition(int winLength) const;
	CellState GetCellState(int x, int y) const;
	void SetCellState(int x, int y, CellState state);
	void ResetBoard(sf::Vector2i boardSize, sf::Vector2f windowSize);
	void Draw(sf::RenderWindow& window);
};