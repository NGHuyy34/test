#include "game.h"
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>

using namespace std;

Game::Game(int mode, Level level, UserManager* userManager) 
    : m_userManager(userManager), m_player1User(nullptr), m_player2User(nullptr)
{
    if (mode == 1) 
    {
        player1 = new Player("Player 1", 'X');
        player2 = new Player("Player 2", 'O');
    } 
    else 
    {
        player1 = new Player("Player", 'X');
        player2 = new Bot('O', level);
    }
    m_player_turn = true;
}

Game::~Game()
{
    delete player1;
    delete player2;
}

void Game::setPlayers(shared_ptr<User> player1User, shared_ptr<User> player2User)
{
    delete player1;
    delete player2;
    
    player1 = new Player(player1User->getUsername(), 'X');
    player2 = new Player(player2User->getUsername(), 'O');
    
    m_player1User = player1User;
    m_player2User = player2User;
}

void Game::saveReplay()
{
    // Tạo tên file replay duy nhất
    auto now = chrono::system_clock::now();
    auto timestamp = chrono::system_clock::to_time_t(now);
    
    m_replayFilename = "replay_" + m_player1User->getUsername() + "_" + m_player2User->getUsername() + "_" 
                        + to_string(timestamp) + ".txt";
    
    ofstream replayFile(m_replayFilename);
    if (!replayFile.is_open()) {
        cout << "Could not create replay file." << endl;
        return;
    }

    // Ghi thông tin người chơi
    replayFile << player1->getName() << "," << player1->getSymbol() << endl;
    replayFile << player2->getName() << "," << player2->getSymbol() << endl;

    // Ghi các nước đi
    for (const auto& move : m_moves) {
        replayFile << move.m_row << "," << move.m_col << "," << move.m_symbol << endl;
    }

    replayFile.close();
    cout << "Replay saved as " << m_replayFilename << endl;
}

void Game::replayGame(const std::string& filename)
{
    ifstream replayFile(filename);
    if (!replayFile.is_open()) {
        cout << "Could not open replay file." << endl;
        return;
    }

    // Đọc thông tin người chơi
    string line;
    getline(replayFile, line);
    istringstream ss1(line);
    string player1Name, player1Symbol;
    getline(ss1, player1Name, ',');
    getline(ss1, player1Symbol);

    getline(replayFile, line);
    istringstream ss2(line);
    string player2Name, player2Symbol;
    getline(ss2, player2Name, ',');
    getline(ss2, player2Symbol);

    // Khởi tạo lại bảng
    m_board = Board();

    // Hiển thị thông tin replay
    cout << "Replay: " << player1Name << " vs " << player2Name << endl;

    // Phát lại các nước đi
    while (getline(replayFile, line)) {
        istringstream ss(line);
        string rowStr, colStr, symbolStr;
        getline(ss, rowStr, ',');
        getline(ss, colStr, ',');
        getline(ss, symbolStr);

        int row = stoi(rowStr);
        int col = stoi(colStr);
        char symbol = symbolStr[0];

        // Thực hiện nước đi
        m_board.makeMove(row, col, symbol);

        // Hiển thị bảng
        system("cls");
        cout << "Replay: " << player1Name << " vs " << player2Name << endl;
        m_board.displayBoard();

        // Delay giữa các nước đi
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    cout << "Replay finished. Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void Game::start()
{
    // Xóa các nước đi cũ
    m_moves.clear();
    cout << resetiosflags(ios::adjustfield);

    while(true)
    {
        system("cls");
        cout << "=> " << player1->getName() << " = " << player1->getSymbol() << ", "
             << player2->getName() << " = " << player2->getSymbol() << endl; 
        cout << endl;
        m_board.displayBoard();

        pair<int, int> move;
        if(m_player_turn)
        {
            move = player1->getMove(m_board);
        }
        else
        {
            move = player2->getMove(m_board);
        }

        char currentSymbol = m_player_turn ? player1->getSymbol() : player2->getSymbol();
        
        // Lưu nước đi
        m_moves.push_back(Move(move.first, move.second, currentSymbol));

        m_board.makeMove(move.first, move.second, currentSymbol);

        if(m_board.checkWin(move.first, move.second, currentSymbol))
        {
            system("cls");
            cout << "=> " << player1->getName() << " = " << player1->getSymbol() << ", "
                 << player2->getName() << " = " << player2->getSymbol() << endl; 
            cout << endl;
            m_board.displayBoard();
            
            // result of 2 players
            if (m_userManager && m_player1User && m_player2User)
            {
                if (m_player_turn)
                {
                    m_player1User->incrementWins();
                    m_player2User->incrementLosses();
                }
                else
                {
                    m_player2User->incrementWins();
                    m_player1User->incrementLosses();
                }

                m_userManager->saveUsersToFile();
                saveReplay();
            }

            cout << (m_player_turn ? player1->getName() : player2->getName()) << " win!" << endl;

            cout << "Press Enter to return menu...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            break;
        }

        if(m_board.checkDraw())
        {
            system("cls");
            cout << "=> " << player1->getName() << " = " << player1->getSymbol() << ", "
                 << player2->getName() << " = " << player2->getSymbol() << endl; 
            cout << endl;
            m_board.displayBoard();
            cout << "Draw!" << endl;
            
            // Cập nhật thông tin hòa cho người chơi
            if (m_userManager && m_player1User && m_player2User)
            {
                m_player1User->incrementDraws();
                m_player2User->incrementDraws();

                m_userManager->saveUsersToFile();
                cout << "Users saved. Player 1 draws: " << m_player1User->getDraws() 
                    << ", Player 2 draws: " << m_player2User->getDraws() << endl;

                saveReplay();
            }

            cout << "Press Enter to return menu...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            break;
        }

        m_player_turn = !m_player_turn;
    }
}