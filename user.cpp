#include "user.h"

User::User(const std::string& username, const std::string& password)
    : m_username(username), m_password(password), m_wins(0), m_losses(0), m_draws(0) {}

std::string User::getUsername() const
{
    return m_username;
}

std::string User::getPassword() const
{
    return m_password;
}

bool User::checkPassword(const std::string& password) const
{
    return m_password == password;
}

void User::incrementWins()
{
    m_wins++;
}

void User::incrementLosses()
{
    m_losses++;
}

void User::incrementDraws()
{
    m_draws++;
}

int User::getWins() const
{
    return m_wins;
}

int User::getLosses() const
{
    return m_losses;
}

int User::getDraws() const
{
    return m_draws;
}

void User::setWins(int wins)
{
    m_wins = wins;
}

void User::setLosses(int losses)
{
    m_losses = losses;
}

void User::setDraws(int draws)
{
    m_draws = draws;
}

