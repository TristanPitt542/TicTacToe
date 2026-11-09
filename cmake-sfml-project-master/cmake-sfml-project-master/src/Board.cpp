/** @file Board.cpp */

#include "Board.hpp"

    // Board cells are stored in row-major order.
Board::Board(sf::Vector2i boardSize, sf::Vector2f windowSize)
{
    m_boardSize = boardSize;
    m_board = std::vector<CellState>(m_boardSize.x * m_boardSize.y, CellState::EMPTY);

    // Use the smaller scale so the full board fits in the window.
    float maxBoardWidth = windowSize.x / m_boardSize.x;
    float maxBoardHeight = windowSize.y / m_boardSize.y;
    m_cellSize = std::min(maxBoardWidth, maxBoardHeight);

    float totalBoardWidth = m_boardSize.x * m_cellSize;
    float totalBoardHeight = m_boardSize.y * m_cellSize;

    // Center the board in the window.
    m_boardOffset.x = (windowSize.x - totalBoardWidth) / 2.f;
    m_boardOffset.y = (windowSize.y - totalBoardHeight) / 2.f;
}

Board::~Board()
{
}

bool Board::HandleMouseClick(sf::Vector2i mousePos, CellState playerPiece)
{
    // Convert window coordinates to board coordinates.
    float relativeX = mousePos.x - m_boardOffset.x;
    float relativeY = mousePos.y - m_boardOffset.y;

    int i = static_cast<int>(relativeX / m_cellSize);
    int j = static_cast<int>(relativeY / m_cellSize);

    if (i >= 0 && i < m_boardSize.x && j >= 0 && j < m_boardSize.y)
    {
        if (GetCellState(i, j) == CellState::EMPTY)
        {
            SetCellState(i, j, playerPiece);
            return true;
        }
    }

    return false; 
}

CellState Board::CheckWinCondition(int winLength) const
{
    // Horizontal, vertical, and diagonal directions.
    const sf::Vector2i directions[] = {
        {1, 0},   // Right (horizontal)
        {0, 1},   // Down (vertical)
        {1, 1},   // Diagonal Down-Right
        {1, -1}   // Diagonal Up-Right
    };

    bool hasEmptyCell = false;

    for (int y = 0; y < m_boardSize.y; ++y)
    {
        for (int x = 0; x < m_boardSize.x; ++x)
        {
            CellState current = GetCellState(x, y);
            if (current == CellState::EMPTY) {
                hasEmptyCell = true;
                continue;  // Skip empty cells
            }

            for (const auto& dir : directions)
            {
                bool matchFound = true;

                for (int i = 1; i < winLength; ++i)
                {
                    if (GetCellState(x + dir.x * i, y + dir.y * i) != current)
                    {
						matchFound = false;
                        break;
					}
				}

				if (matchFound)
				{
					return current;
				}
			}
		}
	}

    // No winning line; a full board is a tie.
	if (!hasEmptyCell)
	{
		return CellState::TIE;
	}

	return CellState::EMPTY;
}

CellState Board::GetCellState(int x, int y) const
{
    // Treat out-of-range lookups as empty; this also simplifies line checks.
	if (x < 0 || x >= m_boardSize.x || y < 0 || y >= m_boardSize.y) {
		return CellState::EMPTY;
	}

	return m_board[y * m_boardSize.x + x];
}

void Board::SetCellState(int x, int y, CellState state)
{
	if (x >= 0 && x < m_boardSize.x && y >= 0 && y < m_boardSize.y) {
		m_board[y * m_boardSize.x + x] = state;
	}
}

void Board::ResetBoard(sf::Vector2i boardSize, sf::Vector2f windowSize)
{
	m_boardSize = boardSize;
	m_board.assign(m_boardSize.x * m_boardSize.y, CellState::EMPTY);

    // Recalculate the scale and offset for the new dimensions.
	float maxBoardWidth = windowSize.x / m_boardSize.x;
	float maxBoardHeight = windowSize.y / m_boardSize.y;
	m_cellSize = std::min(maxBoardWidth, maxBoardHeight);

	float totalBoardWidth = m_boardSize.x * m_cellSize;
	float totalBoardHeight = m_boardSize.y * m_cellSize;

	m_boardOffset.x = (windowSize.x - totalBoardWidth) / 2.f;
	m_boardOffset.y = (windowSize.y - totalBoardHeight) / 2.f;
}

/**
 * @brief Draws the complete board grid and all pieces
 * 
 * Renders the game board including:
 * - Checkerboard grid cells with alternating shades
 * - O pieces as outlined circles
 * - X pieces as diagonal crosses
 * 
 * @param window The render window to draw to
 */
void Board::Draw(sf::RenderWindow& window)
{
    // Existing Cell Background Template
    sf::RectangleShape rect(sf::Vector2f(m_cellSize, m_cellSize));

    // Piece Templates with Padding
    float padding = m_cellSize * 0.15f; // 15% padding from borders
    float pieceSize = m_cellSize - (padding * 2.f);

    // Template for 'O'
    sf::CircleShape oPiece(pieceSize / 2.f);
    oPiece.setFillColor(sf::Color::Transparent);
    oPiece.setOutlineColor(m_oColor);       // Neon Blue for Player O
    oPiece.setOutlineThickness(m_cellSize * 0.08f); // 8% thickness relative to cell size

    // Template for 'X' lines
    float lineThickness = m_cellSize * 0.08f;
    sf::RectangleShape xLine(sf::Vector2f(pieceSize * 1.2f, lineThickness)); // Slightly longer for diagonal cross
    xLine.setFillColor(m_xColor);
    // Set the origin to the center of the line so it rotates cleanly
    xLine.setOrigin({ (pieceSize * 1.2f) / 2.f, lineThickness / 2.f });

    for (int i = 0; i < m_boardSize.x; ++i)
    {
        for (int j = 0; j < m_boardSize.y; ++j)
        {
            // Position calculations for background tile
            float posX = m_boardOffset.x + (i * m_cellSize);
            float posY = m_boardOffset.y + (j * m_cellSize);
            rect.setPosition({ posX, posY });

            // Checkerboard color assignment
            if ((i + j) % 2 == 0) {
                rect.setFillColor(sf::Color(40, 40, 40));
            }
            else {
                rect.setFillColor(sf::Color(60, 60, 60));
            }

            // Draw background cell tile
            window.draw(rect);

            CellState currentCell = GetCellState(i, j);

            if (currentCell == CellState::O)
            {
                // Align O shape factoring in padding
                oPiece.setPosition({ posX + padding, posY + padding });
                window.draw(oPiece);
            }
            else if (currentCell == CellState::X)
            {
                sf::Vector2f cellCenter{ posX + (m_cellSize / 2.f), posY + (m_cellSize / 2.f) };

                // Draw forward slash line
                xLine.setPosition(cellCenter);
                xLine.setRotation(sf::degrees(45.f));
                window.draw(xLine);

                // Draw backslash line
                xLine.setRotation(sf::degrees(135.f));
                window.draw(xLine);
            }
        }
    }
}
