#ifndef __USER_MANAGER_H__
#define __USER_MANAGER_H__

#include "user.h"
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <limits>

class UserManager
{
    private:
        std::vector<std::unique_ptr<User>> m_users;
        std::shared_ptr<User> m_currentUser;
        const std::string USER_FILE = "users.txt";

    public:
        UserManager();
        const std::vector<std::unique_ptr<User>>& getUsers() const;
        bool registerUser(const std::string& username, const std::string& password);
        std::shared_ptr<User> login(const std::string& username, const std::string& password);
        void logout();
        std::shared_ptr<User> getCurrentUser() const;
        bool isUsernameTaken(const std::string& username) const;

        void saveUsersToFile();
        void loadUsersFromFile();
        void findMatchOpponent(std::shared_ptr<User> currentUser);
};

#endif