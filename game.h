#ifndef __GAME_H__
#define __GAME_H__

#include "bot.h"
#include "user_manager.h"
#include "user.h"
#include "move.h"
#include <vector>

class Game
{
private:
    Board m_board;
    Player* player1;
    Player* player2;
    bool m_player_turn;
    UserManager* m_userManager;
    std::shared_ptr<User> m_player1User;
    std::shared_ptr<User> m_player2User;
    std::vector<Move> m_moves; // Lưu trữ các nước đi
    std::string m_replayFilename;

public:
    Game(int mode, Level level = Level::EASY, UserManager* userManager = nullptr);
    ~Game();
    void start();
    void setPlayers(std::shared_ptr<User> player1, std::shared_ptr<User> player2);
    void saveReplay(); // Lưu replay
    void replayGame(const std::string& filename); // Chơi lại game
};

#endif