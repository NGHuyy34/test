#ifndef __MENU_H__
#define __MENU_H__

#include "game.h"
#include "user_manager.h"
#include <vector>
#include <string>

class Menu
{
private:
    UserManager m_userManager;

public:
    void run();
    int showMainMenu();
    int showAuthMenu();
    void showUserAuthMenu();
    void showGameBotMenu();
    int showBotMenu();
    void watchReplay();
    void showPlayerList();
    void searchPlayer();
};

#endif