/** @file Board.hpp */

#pragma once
#include <SFML/Graphics.hpp>

/**
 * @enum CellState
 * @brief Represents the state of a single board cell
 */
enum class CellState { 
	EMPTY,  ///< Cell is unoccupied
	X,      ///< Cell contains an X piece (player)
	O,      ///< Cell contains an O piece (opponent)
	TIE     ///< Game ended in a tie
};

/** Stores board state and handles input, win checks, and rendering. */
class Board
{
private:
	/// Board dimensions in cells (width, height)
	sf::Vector2i m_boardSize;
	/// Size of each individual cell in pixels
	float m_cellSize;
	/// Top-left position of the board in screen coordinates
	sf::Vector2f m_boardOffset;

	/// Game board state - stores the piece in each cell as a flat array
	std::vector<CellState> m_board;

protected:
	// Rendering colors.
	/// Color used to display O pieces on the board
	sf::Color m_oColor = sf::Color::Cyan;
	/// Color used to display X pieces on the board
	sf::Color m_xColor = sf::Color::Magenta;

public:
	/** Creates an empty board sized to the window. */
	Board(sf::Vector2i boardSize, sf::Vector2f windowSize);

	~Board();

	/** Places a piece when the click targets an empty cell. */
	bool HandleMouseClick(sf::Vector2i mousePos, CellState playerPiece);

	/** Returns the winner, a tie, or EMPTY while the game is still active. */
	CellState CheckWinCondition(int winLength) const;

	/** Returns a cell state; out-of-range positions are treated as EMPTY. */
	CellState GetCellState(int x, int y) const;

	/** Updates a cell when the coordinates are in range. */
	void SetCellState(int x, int y, CellState state);

	/** Clears the board and recalculates its layout. */
	void ResetBoard(sf::Vector2i boardSize, sf::Vector2f windowSize);

	/** Draws the cells and pieces. */
	void Draw(sf::RenderWindow& window);
};