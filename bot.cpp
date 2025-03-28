#include "bot.h"
#include <cstdlib>
#include <ctime>
#include <climits>
#include <algorithm>

using namespace std;

Bot::Bot(char symbol, Level level) : Player("Bot", symbol), m_level(level) 
{
    srand(time(0));
}

// Tìm các nước đi tiềm năng với đánh giá điểm số
vector<pair<int, int>> getCandidateMoves(const Board& board, char botSymbol = 'O') {
    vector<pair<int, pair<int, int>>> scored_moves;
    vector<pair<int, int>> moves;

    //char opponentSymbol = (botSymbol == 'X') ? 'O' : 'X';
    char opponentSymbol = 'X';

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board.isValidMove(i, j)) {
                int attackScore = evaluatePosition(board, i, j, botSymbol);
                int defenseScore = evaluatePosition(board, i, j, opponentSymbol);
                
                // Cân nhắc cả tấn công lẫn phòng thủ
                int totalScore = attackScore * 2 + defenseScore;

                // Thêm điểm cho các nước đi gần vị trí đã đánh
                if (board.isAdjacentToSymbol(i, j, botSymbol)) {
                    totalScore += 500;
                }

                scored_moves.push_back({totalScore, {i, j}});
            }
        }
    }

    // Sắp xếp nước đi theo điểm số (giảm dần)
    sort(scored_moves.rbegin(), scored_moves.rend());

    // Giới hạn số nước đi xét đến để tối ưu hiệu suất
    for (int i = 0; i < min(15, (int)scored_moves.size()); i++) {
        moves.push_back(scored_moves[i].second);
    }

    return moves;
}

int evaluatePosition(const Board& board, int row, int col, char symbol) {
    int score = 0;
    int directions[4][2] = {{0,1}, {1,0}, {1,1}, {1,-1}};

    for (auto& dir : directions) {
        int count = 0, openEnds = 0;

        for (int step = -1; step >= -4; step--) {
            int r = row + step * dir[0], c = col + step * dir[1];
            if (r < 0 || r >= SIZE || c < 0 || c >= SIZE) break;
            char cell = board.getCell(r, c);
            if (cell == symbol) count++;
            else if (cell == ' ') { openEnds++; break; }
            else break;
        }

        for (int step = 1; step <= 4; step++) {
            int r = row + step * dir[0], c = col + step * dir[1];
            if (r < 0 || r >= SIZE || c < 0 || c >= SIZE) break;
            char cell = board.getCell(r, c);
            if (cell == symbol) count++;
            else if (cell == ' ') { openEnds++; break; }
            else break;
        }

        if (count == 5) score += 100000;
        else if (count == 4 && openEnds == 2) score += 40000;  // **Tăng điểm cho 4 hở 2 đầu**
        else if (count == 4 && openEnds == 1) score += 15000;  // **Tăng điểm cho 4 hở 1 đầu**
        else if (count == 3 && openEnds == 2) score += 12000;  // **Tăng điểm cho 3 hở 2 đầu**
        else if (count == 3 && openEnds == 1) score += 4000;
    }

    return score;
}

// Đánh giá toàn bộ bàn cờ
int evaluateBoard(const Board& board, char botSymbol) {
    //char opponentSymbol = (botSymbol == 'X') ? 'O' : 'X';
    char opponentSymbol = 'X';
    int score = 0;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            char cell = board.getCell(i, j);
            if (cell == botSymbol) {
                score += evaluatePosition(board, i, j, botSymbol);
                if (board.isAdjacentToSymbol(i, j, botSymbol)) score += 500; // Thêm điểm nếu gần vị trí đã đánh
                if (i > 3 && i < SIZE - 3 && j > 3 && j < SIZE - 3) score += 300; // Kiểm soát trung tâm
            }
            else if (cell == opponentSymbol) {
                score -= evaluatePosition(board, i, j, opponentSymbol);
            }
        }
    }
    return score;
}

// Minimax với Alpha-Beta Pruning
int minimax(Board& board, int depth, bool isMax, int alpha, int beta, char botSymbol) {
    int score = evaluateBoard(board, botSymbol);
   // char opponentSymbol = (botSymbol == 'X') ? 'O' : 'X';
   char opponentSymbol = 'X';

    if (score >= 100000) return 100000;
    if (score <= -100000) return -100000;
    if (depth == 0) return score;

    vector<pair<int, int>> moves = getCandidateMoves(board, botSymbol);
    if (moves.empty()) return 0;

    if (isMax) {
        int best = INT_MIN;
        for (auto [i, j] : moves) {
            board.makeMove(i, j, botSymbol);
            best = max(best, minimax(board, depth - 1, false, alpha, beta, botSymbol));
            board.undoMove(i, j);
            alpha = max(alpha, best);
            if (beta <= alpha) break;
        }
        return best;
    } 
    else {
        int best = INT_MAX;
        for (auto [i, j] : moves) {
            board.makeMove(i, j, opponentSymbol);
            best = min(best, minimax(board, depth - 1, true, alpha, beta, botSymbol));
            board.undoMove(i, j);
            beta = min(beta, best);
            if (beta <= alpha) break;
        }
        return best;
    }
}

pair<int, int> Bot::getMove(const Board& board) {
    vector<pair<int, int>> moves = getCandidateMoves(board, m_symbol);
    char opponentSymbol = 'X';

    if (m_level == Level::EASY) {
        // Nếu có thể chặn chiến thắng của đối thủ, làm ngay
        for (auto [i, j] : moves) {
            Board tempBoard = board;
            tempBoard.makeMove(i, j, opponentSymbol);
            if (tempBoard.checkWin(i, j, opponentSymbol)) return {i, j};
        }
    
        // Lấy danh sách top 10 nước đi tốt nhất
        int topN = min(10, (int)moves.size());
        return moves[rand() % topN];  // Chọn ngẫu nhiên từ top 10
    } 
    else if (m_level == Level::MEDIUM) {
        // Nếu bot đi sau, đặt nước đi gần người chơi
        if (board.getEmptyCells() >= SIZE * SIZE - 1) {
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    if (board.getCell(i, j) == opponentSymbol) {
                        vector<pair<int, int>> adjacentMoves = {
                            {i-1, j}, {i+1, j}, {i, j-1}, {i, j+1},
                            {i-1, j-1}, {i-1, j+1}, {i+1, j-1}, {i+1, j+1}
                        };
                        for (auto [r, c] : adjacentMoves) {
                            if (r >= 0 && r < SIZE && c >= 0 && c < SIZE && board.isValidMove(r, c)) {
                                return {r, c};
                            }
                        }
                    }
                }
            }
        }
    
        // 1. Nếu có thể thắng ngay, chọn luôn
        for (auto [i, j] : moves) {
        Board tempBoard = board;
        tempBoard.makeMove(i, j, m_symbol);
        if (tempBoard.checkWin(i, j, m_symbol)) return {i, j};
        }

        // 2. Nếu đối thủ có thể thắng ngay, chặn ngay
        for (auto [i, j] : moves) {
            Board tempBoard = board;
            tempBoard.makeMove(i, j, opponentSymbol);
            if (tempBoard.checkWin(i, j, opponentSymbol)) return {i, j};
        }
        // 4. Chặn 4 quân hở 2 đầu của đối thủ
        for (auto [i, j] : moves) {
            if (evaluatePosition(board, i, j, opponentSymbol) >= 20000) {
                return {i, j};
            }
        }
        // 5. Nếu có thể tạo 4 quân liên tiếp, thực hiện ngay
        for (auto [i, j] : moves) {
            if (evaluatePosition(board, i, j, m_symbol) >= 20000) {
                return {i, j};
            }
        }
        // 7. Nếu bot có thể tạo nước 3 hở 2 đầu, đánh tiếp
        for (auto [i, j] : moves) {
            if (evaluatePosition(board, i, j, m_symbol) >= 8000) { // open-three
                return {i, j};
            }
        }
        // 10. Nếu không có nước chặn, chọn nước đi tốt nhất bằng Minimax
        int bestScore = INT_MIN;
        pair<int, int> bestMove = {-1, -1};
        int depth = (board.getEmptyCells() < 10) ? 6 : (board.getEmptyCells() < 15) ? 5 : 3;

        moves = vector<pair<int, int>>(moves.begin(), moves.begin() + min(15, (int)moves.size()));

        for (auto [i, j] : moves) {
            Board tempBoard = board;
            tempBoard.makeMove(i, j, m_symbol);
            int moveScore = minimax(tempBoard, depth, false, INT_MIN, INT_MAX, m_symbol);

            if (moveScore > bestScore) {
                bestScore = moveScore;
                bestMove = {i, j};
            }
        }

        return bestMove;
    }    
    else {
        // Nếu bot đi sau, đặt nước đi gần người chơi
        if (board.getEmptyCells() >= SIZE * SIZE - 1) {
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    if (board.getCell(i, j) == opponentSymbol) {
                        vector<pair<int, int>> adjacentMoves = {
                            {i-1, j}, {i+1, j}, {i, j-1}, {i, j+1},
                            {i-1, j-1}, {i-1, j+1}, {i+1, j-1}, {i+1, j+1}
                        };
                        for (auto [r, c] : adjacentMoves) {
                            if (r >= 0 && r < SIZE && c >= 0 && c < SIZE && board.isValidMove(r, c)) {
                                return {r, c};
                            }
                        }
                    }
                }
            }
        }
    
       // 1. Nếu có thể thắng ngay, chọn luôn
        for (auto [i, j] : moves) {
            Board tempBoard = board;
            tempBoard.makeMove(i, j, m_symbol);
            if (tempBoard.checkWin(i, j, m_symbol)) return {i, j};
        }

        // 2. Nếu đối thủ có thể thắng ngay, chặn ngay
        for (auto [i, j] : moves) {
            Board tempBoard = board;
            tempBoard.makeMove(i, j, opponentSymbol);
            if (tempBoard.checkWin(i, j, opponentSymbol)) return {i, j};
        }

        // 3. **Tạo nước đôi** (double threat) -> Ưu tiên nếu có
        for (auto [i, j] : moves) {
            Board tempBoard = board;
            tempBoard.makeMove(i, j, m_symbol);
            int threats = 0;

            // Đếm số đường có thể thắng ngay ở lượt tiếp theo
            for (auto [r, c] : getCandidateMoves(tempBoard, m_symbol)) {
                tempBoard.makeMove(r, c, m_symbol);
                if (tempBoard.checkWin(r, c, m_symbol)) threats++;
                tempBoard.undoMove(r, c);
            }

            if (threats >= 2) return {i, j};  // Chọn nước đi tạo 2 mối đe dọa
        }

        // 4. Chặn 4 quân hở 2 đầu của đối thủ
        for (auto [i, j] : moves) {
            if (evaluatePosition(board, i, j, opponentSymbol) >= 20000) {
                return {i, j};
            }
        }

        // 5. Nếu có thể tạo 4 quân liên tiếp, thực hiện ngay
        for (auto [i, j] : moves) {
            if (evaluatePosition(board, i, j, m_symbol) >= 20000) {
                return {i, j};
            }
        }

        // 6. **Bẫy đối thủ**: Nếu có nước đi khiến đối thủ chỉ có một lựa chọn, tận dụng ngay
        for (auto [i, j] : moves) {
            Board tempBoard = board;
            tempBoard.makeMove(i, j, m_symbol);
            int opponentOptions = 0;
            pair<int, int> forcedMove = {-1, -1};

            for (auto [r, c] : getCandidateMoves(tempBoard, opponentSymbol)) {
                tempBoard.makeMove(r, c, opponentSymbol);
                if (tempBoard.checkWin(r, c, opponentSymbol)) {
                    opponentOptions++;
                    forcedMove = {r, c};
                }
                tempBoard.undoMove(r, c);
            }

            // Nếu đối thủ chỉ có một cách chặn và bot có thể tận dụng lượt sau -> chọn nước đi này
            if (opponentOptions == 1) {
                return {i, j};
            }
        }

        // 7. Nếu bot có thể tạo nước 3 hở 2 đầu, đánh tiếp
        for (auto [i, j] : moves) {
            if (evaluatePosition(board, i, j, m_symbol) >= 8000) { // open-three
                return {i, j};
            }
        }

        // 8. Chặn "double open-three" của đối thủ
        int doubleOpenThreeCount = 0;
        pair<int, int> doubleThreeMove = {-1, -1};
        for (auto [i, j] : moves) {
            if (evaluatePosition(board, i, j, opponentSymbol) >= 8000) {
                doubleOpenThreeCount++;
                doubleThreeMove = {i, j};
            }
        }
        if (doubleOpenThreeCount >= 2) return doubleThreeMove;

        // 9. Chặn 3 quân hở
        for (auto [i, j] : moves) {
            if (evaluatePosition(board, i, j, opponentSymbol) >= 8000) {
                return {i, j}; 
            }
        }

        // 10. Nếu không có nước chặn, chọn nước đi tốt nhất bằng Minimax
        int bestScore = INT_MIN;
        pair<int, int> bestMove = {-1, -1};
        int depth = (board.getEmptyCells() < 15) ? 10 : (board.getEmptyCells() < 20) ? 8 : 5;

        moves = vector<pair<int, int>>(moves.begin(), moves.begin() + min(20, (int)moves.size()));

        for (auto [i, j] : moves) {
            Board tempBoard = board;
            tempBoard.makeMove(i, j, m_symbol);
            int moveScore = minimax(tempBoard, depth, false, INT_MIN, INT_MAX, m_symbol);

            if (moveScore > bestScore) {
                bestScore = moveScore;
                bestMove = {i, j};
            }
        }

        return bestMove;
    }    

    // Nếu không có nước đi nào hợp lệ (trường hợp hiếm)
    return {-1, -1};
}
