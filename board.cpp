#include "board.h"
#include <iomanip>

using namespace std;

Board::Board()
{
    m_board.resize(SIZE, vector<char>(SIZE, ' '));
}

void Board::displayBoard() const
{
    cout << "  ";
    for(int i = 0; i < SIZE; i++)
    {
        cout << setw(4) << i;
    }
    cout << "\n   " << std::string(SIZE * 4 + 1, '-') << "\n";

    for(int i = 0; i < SIZE; i++)
    {
        cout << setw(2) << i << " ";
        for(int j = 0; j < SIZE; j++)
        {
            cout << "| " << m_board[i][j] << " ";
        }
        cout << "|" << endl;

        cout << "   ";
        for(int k = 0; k < SIZE; k++)
        {
            cout << "----";
        }
        cout << "-" << endl;
    }
}

char Board::getCell(int row, int col) const
{
    return m_board[row][col];  // Giả sử m_board là mảng lưu trạng thái bàn cờ
}

int Board::getEmptyCells() const 
{
    int count = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (m_board[i][j] == ' ') count++;
        }
    }
    return count;
}

void Board::undoMove(int row, int col) {
    m_board[row][col] = ' ';  // Trả ô cờ về trạng thái trống
}

bool Board::isAdjacentToSymbol(int row, int col, char symbol) const {
    int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},  // Trên trái, trên, trên phải
        {0, -1},         {0, 1},     // Trái, phải
        {1, -1}, {1, 0}, {1, 1}      // Dưới trái, dưới, dưới phải
    };

    for (auto& dir : directions) {
        int newRow = row + dir[0];
        int newCol = col + dir[1];

        if (newRow >= 0 && newRow < SIZE && newCol >= 0 && newCol < SIZE) {
            if (m_board[newRow][newCol] == symbol) {
                return true;  // Nếu có ô kề cận chứa 'symbol', trả về true
            }
        }
    }
    return false;  // Không có ô kề cận chứa 'symbol'
}

bool Board::isValidMove(int row, int col) const
{
    return row >= 0 && row < SIZE && col >= 0 && col < SIZE && m_board[row][col] == ' ';
}

void Board::makeMove(int row, int col, char symbol)
{
    m_board[row][col] = symbol;
}

bool Board::checkWin(int row, int col, char symbol) const
{
    // Check hàng ngang
    int count = 0;
    for(int i = 0; i < SIZE; i++)
    {
        count = (m_board[row][i] == symbol) ? count + 1 : 0;
        if(count == 5) return true;
    }

    // Check hàng dọc
    count = 0;
    for(int i = 0; i < SIZE; i++)
    {
        count = (m_board[i][col] == symbol) ? count + 1 : 0;
        if(count == 5) return true;
    }

    // Check đường chéo chính
    count = 0;
    for(int i = -4; i <= 4; ++i)
    {
        int r = row + i, c = col + i;
        if(r >= 0 && r < SIZE && c >= 0 && c < SIZE)
        {
            count = (m_board[r][c] == symbol) ? count + 1 : 0; 
            if(count == 5) return true;
        }
    }

    // Check đường chéo phụ
    count = 0;
    for(int i = -4; i <= 4; i++)
    {
        int r = row + i, c = col - i;
        if(r >= 0 && r < SIZE && c >= 0 && c < SIZE)
        {
            count = (m_board[r][c] == symbol) ? count + 1 : 0;
            if(count == 5) return true;
        }
    }

    return false;
}

bool Board::checkDraw() const
{
    for(int i = 0; i < SIZE; i++)
    {
        for(int j = 0; j < SIZE; j++)
        {
            if(m_board[i][j] == ' ') return false;
        }
    }
    return true;
}


