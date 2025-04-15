#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

class AdminManager {
private:
    const string CREDENTIALS_FILE = "admin_credentials.txt";
    vector<pair<string, string>> adminAccounts;

    void loadCredentials() {
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
    void saveCredentials() {
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

public:
    AdminManager() {
        loadCredentials();
    }

    bool addAdminAccount(const string& username, const string& password) {
        auto it = find_if(adminAccounts.begin(), adminAccounts.end(),
            [&username](const pair<string, string>& account) {
                return account.first == username;
            });

        if (it != adminAccounts.end()) {
            cout << "Username already exists!\n";
            return false;
        }

        adminAccounts.push_back({ username, password });
        saveCredentials();
        cout << "Admin account created successfully!\n";
        return true;
    }

    bool validateLogin(const string& username, const string& password) {
        loadCredentials();
        for (const auto& account : adminAccounts) {
            if (account.first == username && account.second == password) {
                return true;
            }
        }
        return false;
    }

    void displayAdminAccounts() {
        loadCredentials();
        if (adminAccounts.empty()) {
            cout << "No admin accounts found.\n";
            return;
        }

        cout << "Registered Admin Accounts:\n";
        for (const auto& account : adminAccounts) {
            cout << "Username: " << account.first << endl;
        }
    }
};

// Function to display the main role menu
void displayMainMenu() {
    cout << "\n=== Welcome to the System ===\n";
    cout << "1. Admin\n";
    cout << "2. User\n";
    cout << "3. Exit\n";
    cout << "Select your role: ";
}

// Function to handle admin options
void handleAdminMenu(AdminManager& adminManager) {
    int adminChoice;
    do {
        cout << "\n--- Admin Management ---\n";
        cout << "1. Create Admin Account\n";
        cout << "2. Login\n";
        cout << "3. View Admin Accounts\n";
        cout << "4. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> adminChoice;

        switch (adminChoice) {
        case 1: {
            string username, password;
            cout << "Enter new admin username: ";
            cin >> username;
            cout << "Enter new admin password: ";
            cin >> password;
            adminManager.addAdminAccount(username, password);
            break;
        }
        case 2: {
            string username, password;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            if (adminManager.validateLogin(username, password)) {
                cout << "Login successful!\n";
                // Add additional admin functionalities here
            }
            else {
                cout << "Invalid credentials!\n";
            }
            break;
        }
        case 3:
            adminManager.displayAdminAccounts();
            break;
        case 4:
            cout << "Returning to main menu...\n";
            break;
        default:
            cout << "Invalid choice!\n";
        }
    } while (adminChoice != 4);
}

// Placeholder function for user role
void handleUserMenu() {
    cout << "\n--- User Section ---\n";
    cout << "User functionalities are not implemented yet.\n";
}

// Main logic separated into a function
void runSystem() {
    AdminManager adminManager;
    int mainChoice;

    do {
        displayMainMenu();
        cin >> mainChoice;

        switch (mainChoice) {
        case 1:
            handleAdminMenu(adminManager);
            break;
        case 2:
            handleUserMenu();
            break;
        case 3:
            cout << "Exiting the system...\n";
            break;
        default:
            cout << "Invalid choice! Try again.\n";
        }
    } while (mainChoice != 3);
}

// Actual main function
int main() {
    runSystem();
    return 0;
}
