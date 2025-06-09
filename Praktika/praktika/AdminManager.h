#pragma once

#include <string>
#include <vector>

class AdminManager {
public:
    AdminManager();
    bool addAdminAccount(const std::string& username, const std::string& password);
    bool validateLogin(const std::string& username, const std::string& password);
    void displayAdminAccounts();
    bool isMainAdmin(const std::string& username, const std::string& password);

    static const std::string MAIN_ADMIN_USERNAME;
    static const std::string MAIN_ADMIN_PASSWORD;

private:
    const std::string CREDENTIALS_FILE = "admin_credentials.txt";
    std::vector<std::pair<std::string, std::string>> adminAccounts;
    void loadCredentials();
    void saveCredentials();
};
