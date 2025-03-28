#ifndef __BOT_H__
#define __BOT_H__

#include "player.h"

enum class Level
{
    EASY, 
    MEDIUM, 
    HARD
};

class Bot : public Player
{
    private:
        Level m_level;
    public:
        Bot(char symbol, Level level);
        std::pair<int, int> getMove(const Board& board) override;
};

int evaluateBoard(const Board& board, char botSymbol);
int evaluatePosition(const Board& board, int row, int col, char symbol);
int minimax(Board& board, int depth, bool isMax, int alpha, int beta, char botSymbol);
//std::vector<std::pair<int, int>> getCandidateMoves(const Board& board);
std::vector<std::pair<int, int>> getCandidateMoves(const Board& board, char botSymbol);

#endif