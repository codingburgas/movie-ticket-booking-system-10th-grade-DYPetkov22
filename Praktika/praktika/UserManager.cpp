#include "UserManager.h"

#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

UserManager::UserManager() {
    loadCredentials();
}

void UserManager::loadCredentials() {
    userAccounts.clear();
    ifstream file(CREDENTIALS_FILE);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        size_t delimiterPos = line.find(':');
        if (delimiterPos != string::npos) {
            string username = line.substr(0, delimiterPos);
            string password = line.substr(delimiterPos + 1);
            userAccounts.push_back({ username, password });
        }
    }
    file.close();
}

void UserManager::saveCredentials() {
    ofstream file(CREDENTIALS_FILE);
    if (!file.is_open()) {
        cout << "Error: Could not save user credentials!\n";
        return;
    }
    for (const auto& account : userAccounts) {
        file << account.first << ":" << account.second << endl;
    }
    file.close();
}

bool UserManager::addUserAccount(const string& username, const string& password) {
    loadCredentials();
    auto it = find_if(userAccounts.begin(), userAccounts.end(),
        [&username](const pair<string, string>& account) {
            return account.first == username;
        });

    if (it != userAccounts.end()) {
        cout << "Username already exists!\n";
        return false;
    }

    userAccounts.push_back({ username, password });
    saveCredentials();
    cout << "User account created successfully!\n";
    return true;
}

bool UserManager::validateLogin(const string& username, const string& password) {
    loadCredentials();
    for (const auto& account : userAccounts) {
        if (account.first == username && account.second == password) {
            return true;
        }
    }
    return false;
}




void UserManager::displayUserAccounts() {
    loadCredentials();
    cout << "Registered User Accounts:\n";
    if (userAccounts.empty()) {
        cout << "No user accounts found.\n";
        return;
    }
    for (const auto& account : userAccounts) {
        cout << "Username: " << account.first << endl;
    }
}
