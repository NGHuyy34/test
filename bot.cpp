#include "bot.h"
#include <cstdlib>
#include <ctime>

using namespace std;

Bot::Bot(char symbol, Level level) : Player("Bot", symbol), m_level(level) 
{
    srand(time(0));
}

pair<int, int> Bot::getMove(const Board& board)
{
    if(m_level == Level::EASY)
    {
        while(true)
        {
            int row = rand() % 10;
            int col = rand() % 10;
            if(board.isValidMove(row, col))
            {
                return make_pair(row, col);
            }
        }
    }

    if(m_level == Level::MEDIUM)
    {
        while(true)
        {
            int row = rand() % 10;
            int col = rand() % 10;
            if(board.isValidMove(row, col))
            {
                return make_pair(row, col);
            }
        }
    }

    if(m_level == Level::HARD)
    {
        while(true)
        {
            int row = rand() % 10;
            int col = rand() % 10;
            if(board.isValidMove(row, col))
            {
                return make_pair(row, col);
            }
        }
    }

    return make_pair(0, 0);
}