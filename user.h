#ifndef __USER_H__
#define __USER_H__

#include <string>
#include <vector>

class User
{
private:
    std::string m_username;
    std::string m_password;
    int m_wins;
    int m_losses;
    int m_draws;

public:
    User(const std::string& username, const std::string& password);
    std::string getUsername() const;
    std::string getPassword() const;
    bool checkPassword(const std::string& password) const;
    
    void incrementWins();
    void incrementLosses();
    void incrementDraws();
    
    int getWins() const;
    int getLosses() const;
    int getDraws() const;
    
    void setWins(int wins);
    void setLosses(int losses);
    void setDraws(int draws);
};

#endif