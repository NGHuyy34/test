#ifndef __BOARD_H__
#define __BOARD_H__

#define SIZE 10

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
        char getCell(int row, int col) const;
        bool isValidMove(int row, int col) const;
        bool checkWin(int row, int col ,char symbol) const;
        bool checkDraw() const;

        int getEmptyCells() const;
        void undoMove(int row, int col);
        bool isAdjacentToSymbol(int row, int col, char symbol) const;
};

#endif