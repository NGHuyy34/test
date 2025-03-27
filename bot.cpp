#include "bot.h"
#include <cstdlib>
#include <ctime>
#include <climits> // Để dùng INT_MIN, INT_MAX

using namespace std;

Bot::Bot(char symbol, Level level) : Player("Bot", symbol), m_level(level) 
{
    srand(time(0));
}

// 🎯 Lấy danh sách các ô hợp lệ gần nước đi trước
vector<pair<int, int>> getCandidateMoves(const Board& board)
{
    vector<pair<int, int>> moves;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (board.isValidMove(i, j))
            {
                // Kiểm tra ô này có gần ô đã đánh không
                for (int di = -2; di <= 2; di++)
                {
                    for (int dj = -2; dj <= 2; dj++)
                    {
                        int ni = i + di, nj = j + dj;
                        if (ni >= 0 && ni < SIZE && nj >= 0 && nj < SIZE && board.getCell(ni, nj) != ' ')
                        {
                            moves.push_back({i, j});
                            goto next_cell; // Thoát loop trong loop
                        }
                    }
                }
            }
            next_cell:;
        }
    }
    return moves;
}

// 🎯 Hàm đánh giá bàn cờ: Tốt hơn Minimax thuần
int evaluateBoard(const Board& board, char botSymbol)
{
    char opponentSymbol = (botSymbol == 'X') ? 'O' : 'X';
    int score = 0;

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (board.getCell(i, j) == botSymbol)
                score += 10;
            else if (board.getCell(i, j) == opponentSymbol)
                score -= 10;
        }
    }

    return score;
}

// 🎯 Minimax với Alpha-Beta Pruning
int minimax(Board& board, int depth, bool isMax, int alpha, int beta, char botSymbol)
{
    int score = evaluateBoard(board, botSymbol);
    if (score >= 1000 || score <= -1000 || depth == 0)
        return score;

    vector<pair<int, int>> moves = getCandidateMoves(board);
    if (moves.empty()) return 0;

    char opponentSymbol = (botSymbol == 'X') ? 'O' : 'X';

    if (isMax) // Lượt bot
    {
        int best = INT_MIN;
        for (auto [i, j] : moves)
        {
            board.makeMove(i, j, botSymbol);
            best = max(best, minimax(board, depth - 1, false, alpha, beta, botSymbol));
            board.makeMove(i, j, ' '); // Hoàn tác nước đi

            alpha = max(alpha, best);
            if (beta <= alpha) break;
        }
        return best;
    }
    else // Lượt đối thủ
    {
        int best = INT_MAX;
        for (auto [i, j] : moves)
        {
            board.makeMove(i, j, opponentSymbol);
            best = min(best, minimax(board, depth - 1, true, alpha, beta, botSymbol));
            board.makeMove(i, j, ' '); // Hoàn tác nước đi

            beta = min(beta, best);
            if (beta <= alpha) break;
        }
        return best;
    }
}

// 🎯 Lấy nước đi của Bot
pair<int, int> Bot::getMove(const Board& board)
{
    if (m_level == Level::EASY)
    {
        // 🟢 BOT EASY: Chọn nước đi ngẫu nhiên
        while (true)
        {
            int row = rand() % SIZE;
            int col = rand() % SIZE;
            if (board.isValidMove(row, col))
            {
                return make_pair(row, col);
            }
        }
    }
    else if (m_level == Level::MEDIUM)
    {
        // 🟠 BOT MEDIUM: Heuristic + tránh nước đi tệ
        vector<pair<int, int>> moves = getCandidateMoves(board);
        pair<int, int> bestMove = {-1, -1};
        int bestScore = INT_MIN;

        for (auto [i, j] : moves)
        {
            Board tempBoard = board;
            tempBoard.makeMove(i, j, m_symbol);
            int moveScore = evaluateBoard(tempBoard, m_symbol);

            if (moveScore > bestScore)
            {
                bestScore = moveScore;
                bestMove = {i, j};
            }
        }

        if (bestMove.first != -1)
            return bestMove;

        // Nếu không có nước đi tốt, chọn random
        while (true)
        {
            int row = rand() % SIZE;
            int col = rand() % SIZE;
            if (board.isValidMove(row, col))
            {
                return make_pair(row, col);
            }
        }
    }
    else // BOT HARD
    {
        // 🔴 BOT HARD: Minimax + Alpha-Beta Pruning
        int depth = 4;
        vector<pair<int, int>> moves = getCandidateMoves(board);
        pair<int, int> bestMove = {-1, -1};
        int bestScore = INT_MIN;

        for (auto [i, j] : moves)
        {
            Board tempBoard = board;
            tempBoard.makeMove(i, j, m_symbol);
            int moveScore = minimax(tempBoard, depth, false, INT_MIN, INT_MAX, m_symbol);

            if (moveScore > bestScore)
            {
                bestScore = moveScore;
                bestMove = {i, j};
            }
        }

        if (bestMove.first != -1)
            return bestMove;

        // Nếu không tìm được nước đi tốt, chọn random
        while (true)
        {
            int row = rand() % SIZE;
            int col = rand() % SIZE;
            if (board.isValidMove(row, col))
            {
                return make_pair(row, col);
            }
        }
    }
}
