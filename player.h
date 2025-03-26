#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "board.h"
#include <string>

class Player
{
    protected:
        std::string m_name;
        char m_symbol;
    public:
        Player(std::string name, char symbol);
        virtual ~Player() = default;
        char getSymbol() const;
        std::string getName() const;
        virtual std::pair<int, int> getMove(const Board &board);
};

#endif