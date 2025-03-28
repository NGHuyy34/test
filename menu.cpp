#include "menu.h"
#include <iostream>
#include <limits>
#include <fstream>
#include <vector>
#include <iomanip> 
#include <algorithm> 

using namespace std;

int Menu::showMainMenu()
{
    cout << "\n===== CARO GAME MAIN MENU =====" << endl;
    cout << "1. Play With Other Player" << endl;
    cout << "2. Play With Bot" << endl;
    cout << "3. Watch Replay" << endl;
    cout << "4. Show Player List" << endl;
    cout << "5. Search Player" << endl;
    cout << "6. Exit" << endl;
    cout << "Choose option: ";
    
    int choice;
    cin >> choice;
    return choice;
}

int Menu::showAuthMenu()
{
    int choice;
    cout << "\n===== USER AUTHENTICATION =====" << endl;
    cout << "1. Register" << endl;
    cout << "2. Login" << endl;
    cout << "3. Return to Main Menu" << endl;
    cout << "Choose option: ";
    cin >> choice;
    return choice;
}

void Menu::showUserAuthMenu()
{
    while (true)
    {
        system("cls");
        int choice = showAuthMenu();

        switch (choice)
        {
        case 1: // Register
        {
            string username, password;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            bool check = m_userManager.registerUser(username, password);
            if(check)
            {
                shared_ptr<User> user = m_userManager.login(username, password);
                m_userManager.findMatchOpponent(user);
            }
            break;
        }
        case 2: // Login
        {
            string username, password;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            shared_ptr<User> user = m_userManager.login(username, password);
            if (user)
            {
                m_userManager.findMatchOpponent(user);
            }
            else
            {
                cout << "\nPress Enter to try again...";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
            }
            break;
        }
        case 3: // Return to Main Menu
            return;
        default:
            cout << "Invalid choice. Please try again.\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    }
}

int Menu::showBotMenu()
{
    int level;
    cout << "\n===== CHOOSE BOT LEVEL =====" << endl;
    cout << "1. Easy" << endl;
    cout << "2. Medium" << endl;
    cout << "3. Hard" << endl;
    cout << "4. Return to Main Menu" << endl;
    cout << "Choose: ";
    cin >> level;
    return level;
}

void Menu::showGameBotMenu()
{
    while (true)
    {
        system("cls");
        int choice = showBotMenu();

        switch (choice)
        {
        case 1: // Player vs Bot Easy
        {
            Game game(2, Level::EASY, &m_userManager);
            game.start();
            break;
        }
        case 2: // Player vs Bot Medium
        {
            Game game(2, Level::MEDIUM, &m_userManager);
            game.start();
            break;
        }
        case 3: // Player vs Bot Hard
        {
            Game game(2, Level::HARD, &m_userManager);
            game.start();
            break;
        }
        case 4: // Return to Main Menu
            return;
        default:
            cout << "Invalid choice. Please try again!\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    }
}

void Menu::watchReplay()
{
    system("cls");
    cout << "\n===== WATCH REPLAY =====" << endl;

    // Liệt kê các file replay
    system("dir replay_*.txt /b > replay_list.txt");
    
    ifstream replayList("replay_list.txt");
    if (!replayList.is_open()) {
        cout << "No replay files found." << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    vector<string> replays;
    string filename;
    int index = 1;
    
    cout << "Available Replays:" << endl;
    while (getline(replayList, filename)) {
        cout << index++ << ". " << filename << endl;
        replays.push_back(filename);
    }
    replayList.close();

    // Xóa file tạm
    system("del replay_list.txt");

    if (replays.empty()) {
        cout << "No replay files found." << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    // Chọn replay để xem
    int choice;
    cout << "Enter replay number to watch (0 to cancel): ";
    cin >> choice;

    if (choice > 0 && choice <= replays.size()) {
        Game game(1); // Tạo game để replay
        game.replayGame(replays[choice - 1]);
    }
}

void Menu::showPlayerList()
{
    system("cls");
    cout << "\n===== REGISTERED PLAYERS LIST =====" << endl;
    
    // Kiểm tra nếu không có người chơi nào
    if (m_userManager.getUsers().empty())
    {
        cout << "No registered players found." << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    // Tiêu đề bảng
    cout << left << setw(20) << "Username" 
         << setw(10) << "Wins" 
         << setw(10) << "Losses" 
         << setw(10) << "Draws" 
         << endl;
    cout << string(50, '-') << endl;

    // Hiển thị thông tin từng người chơi
    for (const shared_ptr<User>& user : m_userManager.getUsers())
    {
        cout << left << setw(20) << user->getUsername()
             << setw(10) << user->getWins()
             << setw(10) << user->getLosses()
             << setw(10) << user->getDraws()
             << endl;
    }

    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void Menu::searchPlayer()
{
    system("cls");
    cout << "\n===== SEARCH PLAYER =====" << endl;

    string searchName;
    cout << "Enter username to search: ";
    cin >> searchName;

    // 🔹 **Sửa chỗ này:** Thay `unique_ptr<User>` → `shared_ptr<User>` trong `find_if`
    auto it = find_if(m_userManager.getUsers().begin(), m_userManager.getUsers().end(), 
        [&searchName](const shared_ptr<User>& user) { 
            return user->getUsername() == searchName; 
        });

    if (it != m_userManager.getUsers().end())
    {
        cout << "\n===== PLAYER DETAILS =====" << endl;
        cout << "Username: " << (*it)->getUsername() << endl;
        cout << "Wins: " << (*it)->getWins() << endl;
        cout << "Losses: " << (*it)->getLosses() << endl;
        cout << "Draws: " << (*it)->getDraws() << endl;

        double totalGames = (*it)->getWins() + (*it)->getLosses() + (*it)->getDraws();
        double winRate = totalGames > 0 ? ((*it)->getWins() / totalGames) * 100 : 0;
        cout << "Win Rate: " << fixed << setprecision(2) << winRate << "%" << endl;
    }
    else
    {
        cout << "Player not found." << endl;
    }

    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void Menu::run()
{
    bool running = true;
    while (running)
    {
        system("cls");
        int choice = showMainMenu();

        switch (choice)
        {
        case 1: // Play With Other Player
            showUserAuthMenu();
            break;
        case 2: // Play With Bot
            showGameBotMenu();
            break;
        case 3: // Watch Replay
            watchReplay();
            break;
        case 4: // Show Player List
            showPlayerList();
            break;
        case 5: // Search Player
            searchPlayer();
            break;
        case 6: // Exit
            cout << "Goodbye!\n";
            running = false;
            break;
        default:
            cout << "Invalid choice. Press Enter to try again!";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    }
}