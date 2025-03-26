#include "player.h"

using namespace std;

Player::Player(string name, char symbol) : m_name(name), m_symbol(symbol) {}

char Player::getSymbol() const
{
    return m_symbol;
}

string Player::getName() const
{
    return m_name;
}

pair<int, int> Player::getMove(const Board& board)
{
    int row, col;
    while(true)
    {
        cout << m_name << "'s turn: ";
        cin >> row >> col;
        if(board.isValidMove(row, col))
        {
            return make_pair(row, col);
        }
        else
        {
            cout << "Invalid move. Try again." << endl;
        }
    }
}