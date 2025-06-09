#include "AdminManager.h"
#include <fstream>
#include <iostream>
#include <algorithm>

const std::string AdminManager::MAIN_ADMIN_USERNAME = "admin";
const std::string AdminManager::MAIN_ADMIN_PASSWORD = "admin123";

using namespace std;

AdminManager::AdminManager() {
    loadCredentials();
}

void AdminManager::loadCredentials() {
    adminAccounts.clear();
    ifstream file(CREDENTIALS_FILE);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        size_t delimiterPos = line.find(':');
        if (delimiterPos != string::npos) {
            string username = line.substr(0, delimiterPos);
            string password = line.substr(delimiterPos + 1);
            adminAccounts.push_back({ username, password });
        }
    }
    file.close();
}

void AdminManager::saveCredentials() {
    ofstream file(CREDENTIALS_FILE);
    if (!file.is_open()) {
        cout << "Error: Could not save credentials!\n";
        return;
    }
    for (const auto& account : adminAccounts) {
        file << account.first << ":" << account.second << endl;
    }
    file.close();
}

bool AdminManager::addAdminAccount(const string& username, const string& password) {
    loadCredentials();
    auto it = find_if(adminAccounts.begin(), adminAccounts.end(),
        [&username](const pair<string, string>& account) {
            return account.first == username;
        });

    if (it != adminAccounts.end() || username == MAIN_ADMIN_USERNAME) {
        cout << "Username already exists or is reserved!\n";
        return false;
    }

    adminAccounts.push_back({ username, password });
    saveCredentials();
    cout << "Admin account created successfully!\n";
    return true;
}

bool AdminManager::validateLogin(const string& username, const string& password) {
    if (username == MAIN_ADMIN_USERNAME && password == MAIN_ADMIN_PASSWORD) {
        return true;
    }
    loadCredentials();
    for (const auto& account : adminAccounts) {
        if (account.first == username && account.second == password) {
            return true;
        }
    }
    return false;
}

void AdminManager::displayAdminAccounts() {
    loadCredentials();
    cout << "Registered Admin Accounts:\n";
    cout << "Username: " << MAIN_ADMIN_USERNAME << " (main admin)\n";
    if (adminAccounts.empty()) {
        cout << "No other admin accounts found.\n";
        return;
    }
    for (const auto& account : adminAccounts) {
        cout << "Username: " << account.first << endl;
    }
}

bool AdminManager::isMainAdmin(const std::string& username, const std::string& password) {
    return username == MAIN_ADMIN_USERNAME && password == MAIN_ADMIN_PASSWORD;
}
