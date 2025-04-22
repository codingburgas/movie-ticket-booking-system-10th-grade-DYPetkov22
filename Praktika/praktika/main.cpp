// Integrated Movie Ticket System with Admin Account Management
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

struct Movie {
    int id;
    string title;
    string genre;
    string language;
    string releaseDate;
};

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

class MovieManager {
private:
    vector<Movie> movies;
    int nextMovieId = 1;
    const string movieFile = "movies.txt";

public:
    MovieManager() {
        loadMoviesFromFile();
        if (movies.empty()) {
            addDefaultMovies();
            saveMoviesToFile();
        }
    }

    void addMovie() {
        Movie m;
        m.id = nextMovieId++;
        cin.ignore();
        cout << "Enter title: "; getline(cin, m.title);
        cout << "Enter genre: "; getline(cin, m.genre);
        cout << "Enter language: "; getline(cin, m.language);
        cout << "Enter release date (YYYY-MM-DD): "; getline(cin, m.releaseDate);
        movies.push_back(m);
        cout << "Movie added successfully!\n";
    }

    void deleteMovie() {
        int id;
        cout << "Enter movie ID to delete: ";
        cin >> id;
        auto it = remove_if(movies.begin(), movies.end(), [id](const Movie& m) { return m.id == id; });
        if (it != movies.end()) {
            movies.erase(it, movies.end());
            cout << "Movie deleted successfully.\n";
        }
        else {
            cout << "Movie ID not found.\n";
        }
    }

    void viewMovies() {
        if (movies.empty()) {
            cout << "No movies available.\n";
            return;
        }
        for (const auto& m : movies) {
            cout << "ID: " << m.id << "\n"
                << "Title: " << m.title << "\n"
                << "Genre: " << m.genre << "\n"
                << "Language: " << m.language << "\n"
                << "Release Date: " << m.releaseDate << "\n"
                << "--------------------------\n";
        }
    }

    void saveMoviesToFile() {
        ofstream out(movieFile);
        for (const auto& m : movies) {
            out << m.id << "|" << m.title << "|" << m.genre << "|" << m.language << "|" << m.releaseDate << "\n";
        }
    }

    void loadMoviesFromFile() {
        ifstream in(movieFile);
        if (!in.is_open()) return;

        string line;
        while (getline(in, line)) {
            stringstream ss(line);
            Movie m;
            string idStr;
            getline(ss, idStr, '|');
            m.id = stoi(idStr);
            getline(ss, m.title, '|');
            getline(ss, m.genre, '|');
            getline(ss, m.language, '|');
            getline(ss, m.releaseDate, '|');
            movies.push_back(m);
            if (m.id >= nextMovieId) nextMovieId = m.id + 1;
        }
    }

    void addDefaultMovies() {
        movies.push_back({ nextMovieId++, "Avengers: Endgame", "Action", "English", "2019-04-26" });
        movies.push_back({ nextMovieId++, "Spirited Away", "Animation", "Japanese", "2001-07-20" });
        movies.push_back({ nextMovieId++, "3 Idiots", "Comedy/Drama", "Hindi", "2009-12-25" });
    }

    void displayMoviesForUser() {
        cout << "\n--- Available Movies ---\n";
        viewMovies();
    }
};

void adminFeatures(MovieManager& movieManager) {
    int choice;
    do {
        cout << "\n--- Admin Functionalities ---\n";
        cout << "1. Add Movie\n";
        cout << "2. Delete Movie\n";
        cout << "3. View All Movies\n";
        cout << "4. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1: movieManager.addMovie(); break;
        case 2: movieManager.deleteMovie(); break;
        case 3: movieManager.viewMovies(); break;
        case 4: movieManager.saveMoviesToFile(); cout << "Logging out...\n"; break;
        default: cout << "Invalid choice!\n";
        }
    } while (choice != 4);
}

void handleAdminMenu(AdminManager& adminManager, MovieManager& movieManager) {
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
                adminFeatures(movieManager);
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

void displayMainMenu() {
    cout << "\n=== Welcome to the System ===\n";
    cout << "1. Admin\n";
    cout << "2. User\n";
    cout << "3. Exit\n";
    cout << "Select your role: ";
}

void handleUserMenu(MovieManager& movieManager) {
    movieManager.displayMoviesForUser();
}

void runSystem() {
    AdminManager adminManager;
    MovieManager movieManager;
    int mainChoice;

    do {
        displayMainMenu();
        cin >> mainChoice;

        switch (mainChoice) {
        case 1:
            handleAdminMenu(adminManager, movieManager);
            break;
        case 2:
            handleUserMenu(movieManager);
            break;
        case 3:
            cout << "Exiting the system...\n";
            break;
        default:
            cout << "Invalid choice! Try again.\n";
        }
    } while (mainChoice != 3);
}

int main() {
    runSystem();
    return 0;
}
