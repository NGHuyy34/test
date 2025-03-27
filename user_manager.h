#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "user.h"
#include <vector>
#include <memory>

class UserManager
{
private:
    std::vector<std::shared_ptr<User>> m_users;
    std::shared_ptr<User> m_currentUser;

public:
    UserManager();
    const std::vector<std::shared_ptr<User>>& getUsers() const;
    
    bool registerUser(const std::string& username, const std::string& password);
    std::shared_ptr<User> login(const std::string& username, const std::string& password);
    void logout();
    
    void findMatchOpponent(std::shared_ptr<User> currentUser);
    
    std::shared_ptr<User> getCurrentUser() const;
    bool isUsernameTaken(const std::string& username) const;

    void saveUsersToFile();
    void loadUsersFromFile();
};

#endif
