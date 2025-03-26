#ifndef __MOVE_H__
#define __MOVE_H__

class Move {
public:
    int m_row;
    int m_col;
    char m_symbol;

    Move(int r, int c, char sym) : m_row(r), m_col(c), m_symbol(sym) {}
};

#endif