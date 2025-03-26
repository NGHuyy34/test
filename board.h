#ifndef __BOARD_H__
#define __BOARD_H__

#include <iostream>
#include <vector>

class Board
{
    private: 
        std::vector<std::vector<char>> m_board;
    public:
        Board();
        void displayBoard() const;
        void makeMove(int row, int col, char symbol);
        bool isValidMove(int row, int col) const;
        bool checkWin(int row, int col ,char symbol) const;
        bool checkDraw() const;
};

#endif