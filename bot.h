#ifndef __BOT_H__
#define __BOT_H__

#include "player.h"

enum class Level
{
    EASY, MEDIUM, HARD
};

class Bot : public Player
{
    private:
        Level m_level;
    public:
        Bot(char symbol, Level level);
        std::pair<int, int> getMove(const Board& board) override;
};

#endif