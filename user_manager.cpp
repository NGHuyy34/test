#include "user_manager.h"
#include "game.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>

using namespace std;

UserManager::UserManager() 
{
    loadUsersFromFile();
}

const std::vector<std::unique_ptr<User>>& UserManager::getUsers() const {
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

    m_users.push_back(unique_ptr<User>(new User(username, password)));
    
    saveUsersToFile();
    
    cout << "Registration successful!" << endl;
    return true;
}

// shared_ptr<User> UserManager::login(const string& username, const string& password)
// {
//     auto it = find_if(m_users.begin(), m_users.end(), 
//         [&username](const unique_ptr<User>& user) { return user->getUsername() == username; });

//     if (it != m_users.end() && (*it)->checkPassword(password))
//     {
//         m_currentUser = shared_ptr<User>(new User(**it));
//         cout << "Login successful!" << endl;
//         return m_currentUser;
//     }

//     cout << "Invalid username or password." << endl;
//     return shared_ptr<User>();
// }

shared_ptr<User> UserManager::login(const string& username, const string& password)
{
    auto it = find_if(m_users.begin(), m_users.end(), 
        [&username](const unique_ptr<User>& user) { return user->getUsername() == username; });

    if (it != m_users.end() && (*it)->checkPassword(password))
    {
        // Không tạo User mới, mà trỏ trực tiếp đến User có sẵn
        m_currentUser = shared_ptr<User>(it->get());  

        cout << "Login successful!" << endl;
        return m_currentUser;
    }

    cout << "Invalid username or password." << endl;
    return shared_ptr<User>();
}


void UserManager::findMatchOpponent(shared_ptr<User> currentUser)
{
    // find opponent with level equivalent
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
                bestOpponent = shared_ptr<User>(user.get());
            }
        }
    }

     // random if can not find opponent
     if (!bestOpponent && !m_users.empty())
     {
         auto it = std::find_if(m_users.begin(), m_users.end(), 
             [&currentUser](const unique_ptr<User>& user) { 
                 return user->getUsername() != currentUser->getUsername(); 
             });
         
         if (it != m_users.end())
         {
             bestOpponent = shared_ptr<User>(new User(**it));
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
        [&username](const unique_ptr<User>& user) { return user->getUsername() == username; });
}

// void UserManager::saveUsersToFile()
// {
//     std::string USER_FILE = "D:\\New folder\\VsCode\\Project\\C_C++\\Mock Project\\users.txt";
//     //D:\\New folder\\VsCode\\Project\\C_C++\\Mock Project
//     //const std::string USER_FILE = "users.txt";

//     ofstream file(USER_FILE, ios::trunc);
//     if (!file.is_open())
//     {
//         cerr << "Error: Could not open file to save users." << endl;
//         return;
//     }

//     cout << "Saving " << m_users.size() << " users..." << endl;

//     for (const unique_ptr<User>& user : m_users)
//     {
//         string line = user->getUsername() + "," + 
//                       user->getPassword() + "," + 
//                       to_string(user->getWins()) + "," + 
//                       to_string(user->getLosses());
        
//         file << line << endl;
        
//         cout << "Saved user: " << line << endl;
//     }

//     file.close();

//     cout << "Users saved successfully." << endl;
// }

void UserManager::saveUsersToFile()
{
    std::string USER_FILE = "users.txt";

    ofstream file(USER_FILE, ios::trunc);
    if (!file.is_open())
    {
        cerr << "Error: Could not open file to save users." << endl;
        return;
    }

    for (const unique_ptr<User>& user : m_users)
    {
        string line = user->getUsername() + "," + 
                      user->getPassword() + "," + 
                      to_string(user->getWins()) + "," + 
                      to_string(user->getLosses()) + "," + 
                      to_string(user->getDraws());
        
        file << line << endl;
    }

    file.close();

    cout << "Users saved successfully to " << USER_FILE << endl;
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

        unique_ptr<User> user(new User(username, password));
        
        user->setWins(atoi(wins_str.c_str()));
        user->setLosses(atoi(losses_str.c_str()));
        user->setDraws(atoi(draws_str.c_str()));

        m_users.push_back(move(user));
    }

    file.close();
}