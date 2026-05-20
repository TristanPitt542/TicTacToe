#pragma once
#include <SFML/Graphics.hpp>

enum class CellState { Empty, X, O };

class Board
{
private:
	sf::Vector2i m_boardSize;
	float m_cellSize;
	sf::Vector2f m_boardOffset;

	std::vector<CellState> m_board;

public:
	Board(sf::Vector2i boardSize, sf::Vector2f widnowSize);
	~Board();

	CellState GetCellState(int x, int y) const;
	void SetCellState(int x, int y, CellState state);
	void ResetBoard();
	void Update();
	void Draw(sf::RenderWindow& window);
};