#include "Board.hpp"

Board::Board(sf::Vector2i boardSize, sf::Vector2f windowSize)
{
    m_boardSize = boardSize;
    m_board = std::vector<CellState>(m_boardSize.x * m_boardSize.y, CellState::Empty);

    // Determine cell size based on the smallest window dimension to ensure it fits completely
    float maxBoardWidth = windowSize.x / m_boardSize.x;
    float maxBoardHeight = windowSize.y / m_boardSize.y;
    m_cellSize = std::min(maxBoardWidth, maxBoardHeight);

    // Calculate the total size of the grid in pixels
    float totalBoardWidth = m_boardSize.x * m_cellSize;
    float totalBoardHeight = m_boardSize.y * m_cellSize;

    // Calculate the centering offset
    m_boardOffset.x = (windowSize.x - totalBoardWidth) / 2.f;
    m_boardOffset.y = (windowSize.y - totalBoardHeight) / 2.f;

}

Board::~Board()
{
}

bool Board::HandleMouseClick(sf::Vector2i mousePos, CellState playerPiece)
{
    // Subtract the offset to get the position relative to the top-left of the board
    float relativeX = mousePos.x - m_boardOffset.x;
    float relativeY = mousePos.y - m_boardOffset.y;

    // Divide by cell size to convert pixels to grid indices
    int i = static_cast<int>(relativeX / m_cellSize);
    int j = static_cast<int>(relativeY / m_cellSize);

    // Boundary check
    if (i >= 0 && i < m_boardSize.x && j >= 0 && j < m_boardSize.y)
    {
        // Move validity check, Only allow placing a piece if the tile is empty
        if (GetCellState(i, j) == CellState::Empty)
        {
            SetCellState(i, j, playerPiece);
            // Click successfully processed and piece placed
            return true;
        }
    }

    // Click was out of bounds or tile was already taken
    return false; 
}

CellState Board::CheckWinCondition() const
{
    // Direction vectors to check {dx, dy}
    // Horizontal, Vertical, Diagonal Down-Right, Diagonal Up-Right
    const sf::Vector2i directions[] = {
        {1, 0},  // Right
        {0, 1},  // Down
        {1, 1},  // Diagonal Down-Right
        {1, -1}  // Diagonal Up-Right
    };

    for (int y = 0; y < m_boardSize.y; ++y)
    {
        for (int x = 0; x < m_boardSize.x; ++x)
        {
            CellState current = GetCellState(x, y);
            if (current == CellState::Empty) {
                continue; // Skip empty cells
            }

            // Look in all 4 directions starting from this cell
            for (const auto& dir : directions)
            {
                // Check if the next two elements in this direction match
                if (GetCellState(x + dir.x, y + dir.y) == current &&
                    GetCellState(x + dir.x * 2, y + dir.y * 2) == current)
                {
                    //Return winner (X or O)
                    return current; 
                }
            }
        }
    }
    // No winner found yet
    return CellState::Empty; 
}

CellState Board::GetCellState(int x, int y) const
{
	// Out of bounds safety check
	if (x < 0 || x >= m_boardSize.x || y < 0 || y >= m_boardSize.y) {
		return CellState::Empty;
	}
	
	return m_board[y * m_boardSize.x + x];
}

void Board::SetCellState(int x, int y, CellState state)
{
	if (x >= 0 && x < m_boardSize.x && y >= 0 && y < m_boardSize.y) {
		m_board[y * m_boardSize.x + x] = state;
	}
}

void Board::ResetBoard()
{
	std::fill(m_board.begin(), m_board.end(), CellState::Empty);
}

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
    oPiece.setOutlineColor(sf::Color::Cyan);       // Neon Blue for Player O
    oPiece.setOutlineThickness(m_cellSize * 0.08f); // 8% thickness relative to cell size

    // Template for 'X' lines
    float lineThickness = m_cellSize * 0.08f;
    sf::RectangleShape xLine(sf::Vector2f(pieceSize * 1.2f, lineThickness)); // Slightly longer for diagonal cross
    xLine.setFillColor(sf::Color::Magenta);
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
                // Find the absolute center point of the current cell square
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
