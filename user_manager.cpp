#include "user_manager.h"
#include "game.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>

using namespace std;

const std::string USER_FILE = "users.txt";  // Đường dẫn file lưu danh sách người dùng

UserManager::UserManager() 
{
    loadUsersFromFile();
}

const std::vector<std::shared_ptr<User>>& UserManager::getUsers() const {
    return m_users;
}

bool UserManager::registerUser(const string& username, const string& password)
{
    if (isUsernameTaken(username))
    {
        cout << "Username already exists. Please choose another." << endl;
        cout << "\nPress Enter to try again...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return false;
    }

    m_users.push_back(make_shared<User>(username, password));
    
    saveUsersToFile();
    
    cout << "Registration successful!" << endl;
    return true;
}

shared_ptr<User> UserManager::login(const string& username, const string& password)
{
    auto it = find_if(m_users.begin(), m_users.end(), 
        [&username](const shared_ptr<User>& user) { return user->getUsername() == username; });

    if (it != m_users.end() && (*it)->checkPassword(password))
    {
        m_currentUser = *it; // Giữ shared_ptr, không trỏ vào raw pointer

        cout << "Login successful!" << endl;
        return m_currentUser;
    }

    cout << "Invalid username or password." << endl;
    return nullptr;
}

void UserManager::findMatchOpponent(shared_ptr<User> currentUser)
{
    if (!currentUser)
    {
        cout << "Error: No logged-in user to find an opponent for!" << endl;
        return;
    }

    double currentUserWinRate = currentUser->getWins() > 0 
        ? static_cast<double>(currentUser->getWins()) / (currentUser->getWins() + currentUser->getLosses()) 
        : 0.5;

    shared_ptr<User> bestOpponent = nullptr;
    double bestDifference = numeric_limits<double>::max();

    for (const auto& user : m_users)
    {
        if (user->getUsername() != currentUser->getUsername())
        {
            double opponentWinRate = (user->getWins() + user->getLosses() > 0)
                ? static_cast<double>(user->getWins()) / (user->getWins() + user->getLosses())
                : 0.5;
            
            double difference = abs(currentUserWinRate - opponentWinRate);
            
            if (difference < bestDifference)
            {
                bestDifference = difference;
                bestOpponent = user;
            }
        }
    }

    if (!bestOpponent && !m_users.empty())
    {
        auto it = find_if(m_users.begin(), m_users.end(), 
            [&currentUser](const shared_ptr<User>& user) { 
                return user->getUsername() != currentUser->getUsername(); 
            });
        
        if (it != m_users.end())
        {
            bestOpponent = *it;
        }
    }

    if (bestOpponent)
    {
        cout << "Matched with opponent: " << bestOpponent->getUsername() << endl;
        Game game(1, Level::EASY, this);
        game.setPlayers(currentUser, bestOpponent);
        game.start();
    }
    else
    {
        cout << "No suitable opponent found." << endl;
    }
}

void UserManager::logout()
{
    m_currentUser.reset();
    cout << "Logged out successfully." << endl;
}

shared_ptr<User> UserManager::getCurrentUser() const
{
    return m_currentUser;
}

bool UserManager::isUsernameTaken(const string& username) const
{
    return any_of(m_users.begin(), m_users.end(), 
        [&username](const shared_ptr<User>& user) { return user->getUsername() == username; });
}

void UserManager::saveUsersToFile()
{
    ofstream file(USER_FILE, ios::trunc);
    if (!file.is_open())
    {
        cerr << "Error: Could not open file to save users." << endl;
        return;
    }

    for (const shared_ptr<User>& user : m_users)
    {
        file << user->getUsername() << "," 
             << user->getPassword() << ","
             << user->getWins() << ","
             << user->getLosses() << ","
             << user->getDraws() << endl;
    }

    file.close();
}

void UserManager::loadUsersFromFile()
{
    ifstream file(USER_FILE);
    if (!file.is_open())
    {
        return;
    }

    m_users.clear();

    string line;
    while (getline(file, line))
    {
        istringstream ss(line);
        string username, password, wins_str, losses_str, draws_str;

        getline(ss, username, ',');
        getline(ss, password, ',');
        getline(ss, wins_str, ',');
        getline(ss, losses_str, ',');
        getline(ss, draws_str);

        shared_ptr<User> user = make_shared<User>(username, password);
        user->setWins(stoi(wins_str));
        user->setLosses(stoi(losses_str));
        user->setDraws(stoi(draws_str));

        m_users.push_back(user);
    }

    file.close();
}