#pragma once

#include <string>
#include <vector>

class UserManager {
public:
    UserManager();
    bool addUserAccount(const std::string& username, const std::string& password);
    bool validateLogin(const std::string& username, const std::string& password);
    void displayUserAccounts();

private:
    const std::string CREDENTIALS_FILE = "user_credentials.txt";
    std::vector<std::pair<std::string, std::string>> userAccounts;
    void loadCredentials();
    void saveCredentials();
};