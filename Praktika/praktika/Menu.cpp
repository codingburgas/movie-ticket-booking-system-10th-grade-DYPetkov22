#include "Menu.h"
#include <iostream>
#include <limits>

using namespace std;

#ifdef _WIN32
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif

void adminFeatures(MovieManager& movieManager, bool isMainAdmin, AdminManager& adminManager) {
    int choice;
    do {
        system(CLEAR_SCREEN);
        cout << "\n--- Admin Functionalities ---\n";
        cout << "1. Add Movie\n";
        cout << "2. Delete Movie\n";
        cout << "3. View All Movies\n";
        cout << "4. View Movie Suggestions\n";
        cout << "5. Approve a Suggestion\n";
        cout << "6. Reject a Suggestion\n";
        cout << "7. Add Projection\n";
        cout << "8. View Projections\n";
        cout << "9. View Purchases for a Movie\n";


        if (isMainAdmin) {
            cout << "10. Create New Admin Account\n";
            cout << "11. Logout\n";
        }
        else {
            cout << "10. Logout\n";
        }
        cout << "Enter choice: ";
        cin >> choice;

        if (isMainAdmin) {
            switch (choice) {
            case 1: movieManager.addMovie(); break;
            case 2: movieManager.deleteMovie(); break;
            case 3: movieManager.viewMovies(); break;
            case 4: movieManager.viewSuggestions(); break;
            case 5: movieManager.approveSuggestion(); break;
            case 6: movieManager.rejectSuggestion(); break;
            case 7: movieManager.addProjection(); break;
            case 8: movieManager.viewProjections(); break;
			case 9: movieManager.viewPurchasesForMovie(); break;
            case 10: {
                string username, password;
                cout << "Enter new admin username: ";
                cin >> username;
                cout << "Enter new admin password: ";
                cin >> password;
                adminManager.addAdminAccount(username, password);
                break;
            }
            case 11: movieManager.saveMoviesToFile(); cout << "Logging out...\n"; break;
            default: cout << "Invalid choice!\n";
            }
        }
        else {
            switch (choice) {
            case 1: movieManager.addMovie(); break;
            case 2: movieManager.deleteMovie(); break;
            case 3: movieManager.viewMovies(); break;
            case 4: movieManager.viewSuggestions(); break;
            case 5: movieManager.approveSuggestion(); break;
            case 6: movieManager.rejectSuggestion(); break;
            case 7: movieManager.addProjection(); break;
            case 8: movieManager.viewProjections(); break;
            case 9: movieManager.viewPurchasesForMovie(); break;
            case 10: movieManager.saveMoviesToFile(); cout << "Logging out...\n"; break;
            default: cout << "Invalid choice!\n";
            }
        }
        cout << "Press Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    } while ((isMainAdmin && choice != 11) || (!isMainAdmin && choice != 10));
}

void adminLoginFlow(AdminManager& adminManager, MovieManager& movieManager) {
    int adminChoice;
    do {
        system(CLEAR_SCREEN);
        cout << "\n--- Admin Login ---\n";
        cout << "1. Login\n";
        cout << "2. View Admin Accounts\n";
        cout << "3. Back\n";
        cout << "Enter your choice: ";
        cin >> adminChoice;

        switch (adminChoice) {
        case 1: {
            string username, password;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            if (adminManager.validateLogin(username, password)) {
                cout << "Login successful!\n";
                bool isMain = adminManager.isMainAdmin(username, password);
                adminFeatures(movieManager, isMain, adminManager);
                return; // After logout, return to login page
            }
            else {
                cout << "Invalid credentials!\n";
            }
            break;
        }
        case 2:
            adminManager.displayAdminAccounts();
            cout << "Press Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            break;
        case 3:
            cout << "Returning to login page...\n";
            break;
        default:
            cout << "Invalid choice!\n";
            break;
        }
    } while (adminChoice != 3);
}

void userMenu(MovieManager& movieManager, const std::string& username) {
    int choice;
    do {
        system(CLEAR_SCREEN);
        cout << "\n------------------ USER MENU ------------------\n| 1. View Movies        | 2. Choose Movie      |\n| 3. Suggest a Movie    | 4. My Purchases      |\n| 5. Logout                                    |\n------------------------------------------------\nEnter your choice: ";

		cin >> choice;
        switch (choice) {
        case 1:
            movieManager.displayMoviesForUser();
            break;
        case 2:
            movieManager.chooseMovieToWatch(username);
            break;
        case 3:
            movieManager.suggestMovie();
            break;
        case 4:
            movieManager.viewAndDeleteMyPurchases(username); 
            break;
        case 5:
            cout << "Logging out...\n";
            break;
        default:
            cout << "Invalid choice!\n";
            break;
        }
        if (choice != 5) {
            cout << "Press Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    } while (choice != 5);
}

void userLoginFlow(UserManager& userManager, MovieManager& movieManager) {
    int userChoice;
    do {
        system(CLEAR_SCREEN);
        cout << "\n******************************************\n";
        cout << "             USER LOGIN MENU            \n";
        cout << "******************************************\n";
        cout << "  1. Login\n";
        cout << "  2. Create User Account \n";
        cout << "  3. Back \n";
        cout << "******************************************\n";
        cout << "  Enter your choice: ";
        cin >> userChoice;

        switch (userChoice) {
        case 1: {
            string username, password;
            cout << endl;
            cout << "  Enter username: ";
            cin >> username;
            cout << endl;
            cout << "  Enter password: ";
            cin >> password;
            if (userManager.validateLogin(username, password)) {
                cout << "Login successful!\n";
                userMenu(movieManager, username);
                return; 
            }
            else {
                cout << "Invalid credentials!\n";
            }
            break;
        }
        case 2: {
            string username, password;
            cout << "Enter new user username: ";
            cin >> username;
            cout << "Enter new user password: ";
            cin >> password;
            userManager.addUserAccount(username, password);
            break;
        }
        case 3:
            cout << "Returning to login page...\n";
            break;
        default:
            cout << "Invalid choice!\n";
            break;
        }
        cout << "Press Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    } while (userChoice != 3);
}

void guestMenu(MovieManager& movieManager) {
    int choice;
    do {
        system(CLEAR_SCREEN);
        cout << "\n  Guest Menu\n";
        cout << "\n---------------------------\n";
        cout << "1. View Movies\n";
        cout << "2. Back";
        cout << "\n---------------------------\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            movieManager.displayMoviesForUser();
            break;
        case 2:
            cout << "Returning to main menu...\n";
            break;
        default:
            cout << "Invalid choice!\n";
            break;
        }
        if (choice != 2) {
            cout << "Press Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    } while (choice != 2);
}

void loginPage() {
    system(CLEAR_SCREEN);
    cout << R"( 
        _ _ _     _                      _          _____ _ _       _____ _           
       | | | |___| |___ ___ _____ ___   | |_ ___   |   __|_| |_____|   __| |___ _ _ _ 
       | | | | -_| |  _| . |     | -_|  |  _| . |  |   __| | |     |   __| | . | | | |
       |_____|___|_|___|___|_|_|_|___|  |_| |___|  |__|  |_|_|_|_|_|__|  |_|___|_____|                                                                              
)" << endl;
    cout << "       1. Admin Login\n";
    cout << "       2. User Login\n";
    cout << "       3. View Movies (Guest)\n";
    cout << "       4. Exit\n\n";
    cout << "       Select your role: ";
}

void runSystem() {
    AdminManager adminManager;
    UserManager userManager;
    MovieManager movieManager;
    int choice;

    do {
        loginPage();
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

        switch (choice) {
        case 1:
            adminLoginFlow(adminManager, movieManager);
            break;
        case 2:
            userLoginFlow(userManager, movieManager);
            break;
        case 3:
            guestMenu(movieManager);
            break;
        case 4:
            cout << "Exiting the system...\n";
            break;
        default:
            cout << "Invalid choice! Try again.\n";
            cout << "Press Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    } while (choice != 4);
}
