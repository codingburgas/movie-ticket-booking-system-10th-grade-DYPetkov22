#include "MovieManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>

using namespace std;



MovieManager::MovieManager() {
    loadMoviesFromFile();
    loadProjectionsFromFile();
    if (movies.empty()) {
        addDefaultMovies();
        saveMoviesToFile();
    }
    loadSuggestions();
}

void MovieManager::addMovie() {
    Movie m;
    m.id = nextMovieId++;
    cin.ignore();
    cout << "Enter title: "; getline(cin, m.title);
    cout << "Enter genre: "; getline(cin, m.genre);
    cout << "Enter language: "; getline(cin, m.language);
    cout << "Enter release date (YYYY-MM-DD): "; getline(cin, m.releaseDate);
    movies.push_back(m);
    saveMoviesToFile();
    cout << "Movie added successfully!\n";
}

void MovieManager::deleteMovie() {
    int id;
	system("cls");
    viewMovies();
    if (movies.empty()) {
        cout << "No movies available to delete.\n";
        return;
	}
	cout << "\n--------------------------------------\n";   
    cout << "Enter movie ID to delete: ";
    cin >> id;

    // Check if there are projections for this movie
    bool hasProjections = false;
    for (const auto& p : projections) {
        if (p.movieId == id) {
            hasProjections = true;
            break;
        }
    }

    if (hasProjections) {
        std::cout << "Cannot delete this movie. It still has projections.\n";
        return;
    }

    // Try to remove the movie
    auto it = std::remove_if(movies.begin(), movies.end(),
        [id](const Movie& m) { return m.id == id; });

    if (it != movies.end()) {
        movies.erase(it, movies.end());
        saveMoviesToFile();
        std::cout << "Movie deleted successfully.\n";
    }
    else {
        std::cout << "Movie ID not found.\n";
    }
}


void MovieManager::viewMovies() {
    system("cls");
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

void MovieManager::viewProjections() {
    if (projections.empty()) {
        cout << "No projections available.\n";
        return;
    }
    cout << "\n--- All Projections ---\n";
    for (const auto& p : projections) {
        // Find movie title for display
        auto it = find_if(movies.begin(), movies.end(), [&p](const Movie& m) { return m.id == p.movieId; });
        string movieTitle = (it != movies.end()) ? it->title : "Unknown";
        cout << "Movie: " << movieTitle << " | Time: " << p.time << " | Format: " << p.format << " | Available Seats: " << p.availableSeats << "\n";
    }
}

void MovieManager::addProjection() {
    if (movies.empty()) {
        cout << "No movies available. Add movies first.\n";
        return;
    }
    int movieId;
    cout << "Enter Movie ID to add projection to:\n";
    viewMovies();
    cout << "Movie ID: ";
    cin >> movieId;
    auto it = find_if(movies.begin(), movies.end(), [movieId](const Movie& m) { return m.id == movieId; });
    if (it == movies.end()) {
        cout << "Invalid Movie ID.\n";
        return;
    }
    Projection p;
    p.movieId = movieId;
    cin.ignore();
    cout << "Enter projection time (e.g. 2025-07-25 18:30): ";
    getline(cin, p.time);
    cout << "Enter format (e.g. 2D, 3D): ";
    getline(cin, p.format);
    cout << "Enter number of available seats: ";
    cin >> p.availableSeats;

    projections.push_back(p);
    saveProjectionsToFile();
    cout << "Projection added successfully.\n";
}

void MovieManager::saveMoviesToFile() {
    ofstream out(movieFile);
    for (const auto& m : movies) {
        out << m.id << "|" << m.title << "|" << m.genre << "|" << m.language << "|" << m.releaseDate << "\n";
    }
}

void MovieManager::loadMoviesFromFile() {
    movies.clear();
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

void MovieManager::saveProjectionsToFile() {
    ofstream out(projectionFile);
    for (const auto& p : projections) {
        out << p.movieId << "|" << p.time << "|" << p.format << "|" << p.availableSeats << "\n";
    }
}

void MovieManager::loadProjectionsFromFile() {
    projections.clear();
    ifstream in(projectionFile);
    if (!in.is_open()) return;

    string line;
    while (getline(in, line)) {
        stringstream ss(line);
        Projection p;
        string seatsStr;
        getline(ss, seatsStr, '|');
        p.movieId = stoi(seatsStr);
        getline(ss, p.time, '|');
        getline(ss, p.format, '|');
        getline(ss, seatsStr, '|');
        p.availableSeats = stoi(seatsStr);
        projections.push_back(p);
    }
}

void MovieManager::logPurchase(const string& buyer,
    const Projection& proj,
    const Movie& mov,
    const string& seatCode)
{
    ofstream out(purchaseFile, ios::app);
    if (!out.is_open()) return;

    out << buyer << '|'
        << mov.title << '|'
        << proj.time << '|'
        << proj.format << '|'
        << proj.availableSeats << '|'
        << seatCode << '\n';
}

void MovieManager::displayMoviesForUser() {
    cout << "\n--- Available Movies ---\n";
    viewMovies();
}

void MovieManager::viewAndDeleteMyPurchases(const string& username) {

    system("cls");
    ifstream in("purchases.txt");
    if (!in) {
        cout << "No purchases found.\n";
        return;
    }

    struct Purchase {
        string title;
        string movieId;
        string time;
        string format;
        string seatCode;
        int seats;
        string rawLine;
    };

    vector<Purchase> userPurchases;
    vector<string> allLines;

    string line;
    while (getline(in, line)) {
        allLines.push_back(line);
        stringstream ss(line);
        string user, movieId, title, time, format, seatsStr, seatCode;

        getline(ss, user, '|');
        getline(ss, title, '|');
        getline(ss, time, '|');
        getline(ss, format, '|');
        getline(ss, seatsStr, '|');   
        getline(ss, seatCode);        

        if (user == username) {
            Purchase p;   
            p.title = title;
            p.movieId = movieId;
            p.time = time;
            p.format = format;
            p.seatCode = seatCode;
            try {
                p.seats = stoi(seatsStr);
            }
            catch (const invalid_argument&) {
                p.seats = 1;
            }
            p.rawLine = line;
            userPurchases.push_back(p);
        }
    }
    in.close();

    if (userPurchases.empty()) {
        cout << "You have no purchases.\n";
        return;
    }

    cout << "\nYour Purchases:\n";
	cout << "--------------------------\n";
    for (size_t i = 0; i < userPurchases.size(); ++i) {
        cout << i + 1 << ". Movie: " << userPurchases[i].movieId << userPurchases[i].title
            << ", Time: " << userPurchases[i].time
            << ", Format: " << userPurchases[i].format
            << ", Seat: " << userPurchases[i].seatCode << "\n";  
    }

    cout << "\nDo you want to delete one? Enter number (0 to cancel): ";
    int choice;
    cin >> choice;
    if (choice < 1 || choice >(int)userPurchases.size()) {
        cout << "Cancelled or invalid.\n";
        return;
    }

    string toDelete = userPurchases[choice - 1].rawLine;

    ofstream out("purchases.txt");
    for (const string& l : allLines) {
        if (l != toDelete) out << l << "\n";
    }

    cout << "Purchase deleted successfully.\n";
}




bool MovieManager::chooseMovieToWatch(const string& username) {
    if (movies.empty()) {
        cout << "No movies available to watch.\n";
        return false;
    }

    viewMovies();
    cout << "Enter the ID of the movie you want to watch: ";
    int id;
    cin >> id;

    // Find projections for that movie
    vector<Projection> movieProjections;
    for (const auto& p : projections) {
        if (p.movieId == id) movieProjections.push_back(p);
    }

    if (movieProjections.empty()) {
        cout << "No projections available for this movie.\n";
        return false;
    }

    cout << "\nAvailable projections:\n";
    for (size_t i = 0; i < movieProjections.size(); ++i) {
        cout << i + 1 << ". Time: " << movieProjections[i].time
            << ", Format: " << movieProjections[i].format
            << ", Available Seats: " << movieProjections[i].availableSeats << "\n";
    }

    cout << "Choose a projection (1-" << movieProjections.size() << "): ";
    int choice;
    cin >> choice;
    if (choice < 1 || choice >(int)movieProjections.size()) {
        cout << "Invalid choice.\n";
        return false;
    }

    Projection& selected = movieProjections[choice - 1];

    // Get booked seats for the projection
    auto it = find_if(movies.begin(), movies.end(), [id](const Movie& m) { return m.id == id; });
    if (it == movies.end()) {
        cout << "Movie not found.\n";
        return false;
    }

    set<string> bookedSeats = getBookedSeatsForProjection(it->title, selected.time, selected.format);

    // Calculate total seats for this projection (available + booked)
    int totalSeats = selected.availableSeats + (int)bookedSeats.size();

    if ((int)bookedSeats.size() >= totalSeats) {
        cout << "All seats are booked for this projection.\n";
        return false;
    }

    // Ask user to choose seat
    string chosenSeat = chooseSeat(totalSeats, bookedSeats);

    // Decrease available seats count
    selected.availableSeats--;

    // Update projection in memory
    for (auto& p : projections) {
        if (p.movieId == selected.movieId && p.time == selected.time && p.format == selected.format) {
            p.availableSeats = selected.availableSeats;
            break;
        }
    }
    saveProjectionsToFile();

    cout << "\nNow playing: " << it->title
        << " (" << selected.time << ", " << selected.format << ")\n";
    cout << "Your seat: " << chosenSeat << "\nEnjoy your movie!\n";

    // Log purchase with seat
    logPurchase(username, selected, *it, chosenSeat);
    return true;
}


void MovieManager::saveSuggestions() {
    ofstream out(suggestionFile);
    for (const auto& m : suggestions) {
        out << m.id << "|" << m.title << "|" << m.genre << "|" << m.language << "|" << m.releaseDate << "\n";
    }
}
void MovieManager::viewPurchasesForMovie()
{
    if (movies.empty()) {
        std::cout << "No movies in the system.\n";
        return;
    }

    viewMovies();
    cout << "Enter movie ID to list its purchases: ";
    int id;
    std::cin >> id;

    // Find movie title by id
    auto it = std::find_if(movies.begin(), movies.end(),
        [id](const Movie& m) { return m.id == id; });
    if (it == movies.end()) {
        std::cout << "Movie not found.\n";
        return;
    }
    std::string movieTitle = it->title;

    std::ifstream in(purchaseFile);
    if (!in.is_open()) {
        std::cout << "No purchases found (file missing).\n";
        return;
    }

    bool found = false;
    std::string line;
    std::cout << "\n--- Purchases for movie \"" << movieTitle << "\" ---\n";
    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string buyer, purchaseMovieTitle, time, format, availableSeatsStr, seatCode;

        std::getline(ss, buyer, '|');
        std::getline(ss, purchaseMovieTitle, '|');
        std::getline(ss, time, '|');
        std::getline(ss, format, '|');
        std::getline(ss, availableSeatsStr, '|');
        std::getline(ss, seatCode);

        if (purchaseMovieTitle == movieTitle) {
            found = true;
            std::cout << "Buyer: " << buyer
                << " | Time: " << time
                << " | Format: " << format
                << " | Seat: " << seatCode << '\n';
        }
    }

    if (!found)
    {
        std::cout << "No tickets bought for this movie yet.\n";
        std::cout << "Do you want to delete a projection for this movie? (y/n): ";
        char response;
        std::cin >> response;
        if (response == 'y' || response == 'Y') {
            std::vector<int> projectionIndexes;

            std::cout << "\nAvailable Projections:\n";
            int count = 1;
            for (size_t i = 0; i < projections.size(); ++i) {
                if (projections[i].movieId == id) {
                    projectionIndexes.push_back(i);
                    std::cout << count << ". Time: " << projections[i].time
                        << ", Format: " << projections[i].format
                        << ", Available Seats: " << projections[i].availableSeats << "\n";
                    count++;
                }
            }

            if (projectionIndexes.empty()) {
                std::cout << "No projections found for this movie.\n";
                return;
            }

            std::cout << "Enter the number of the projection to delete (1-" << projectionIndexes.size() << "): ";
            int choice;
            std::cin >> choice;
            if (choice >= 1 && choice <= (int)projectionIndexes.size()) {
                projections.erase(projections.begin() + projectionIndexes[choice - 1]);
                saveProjectionsToFile();
                std::cout << "Projection deleted successfully.\n";
            }
            else {
                std::cout << "Invalid selection.\n";
            }
        }
    }
}

void MovieManager::loadSuggestions() {
    suggestions.clear();
    ifstream in(suggestionFile);
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
        suggestions.push_back(m);
        if (m.id >= nextSuggestionId) nextSuggestionId = m.id + 1;
    }
}

void MovieManager::suggestMovie() {
    Movie m;
    m.id = nextSuggestionId++;
    cin.ignore();
	system("cls");
    cout << "\n--------------------------------------\n";
    cout << "|         Suggest a New Movie        |\n";
    cout << "--------------------------------------\n";
    cout << "Enter title: "; getline(cin, m.title);
    cout << "Enter genre: "; getline(cin, m.genre);
    cout << "Enter language: "; getline(cin, m.language);
    cout << "Enter release date (YYYY-MM-DD): "; getline(cin, m.releaseDate);
    suggestions.push_back(m);
    saveSuggestions();
    cout << "Thank you! Your suggestion will be reviewed by an admin.\n";
}

void MovieManager::viewSuggestions() {
    loadSuggestions();
    if (suggestions.empty()) {
        cout << "No movie suggestions pending.\n";
        return;
    }
    cout << "\n--- Pending Movie Suggestions ---\n";
    for (const auto& m : suggestions) {
        cout << "Suggestion ID: " << m.id << "\n"
            << "Title: " << m.title << "\n"
            << "Genre: " << m.genre << "\n"
            << "Language: "
            << m.language << "\n"
            << "Release Date: " << m.releaseDate << "\n"
            << "--------------------------\n";
    }
}

    void MovieManager::approveSuggestion() {
        loadSuggestions();
        if (suggestions.empty()) {
            cout << "No suggestions to approve.\n";
            return;
        }
        int id;
        cout << "Enter the Suggestion ID to approve: ";
        cin >> id;
        auto it = find_if(suggestions.begin(), suggestions.end(), [id](const Movie& m) { return m.id == id; });
        if (it != suggestions.end()) {
            // Add to movies and remove from suggestions
            Movie m = *it;
            m.id = nextMovieId++;
            movies.push_back(m);
            suggestions.erase(it);
            saveMoviesToFile();
            saveSuggestions();
            cout << "Suggestion approved and added to movies!\n";
        }
        else {
            cout << "Suggestion ID not found.\n";
        }
    }

    void MovieManager::rejectSuggestion() {
        loadSuggestions();
        if (suggestions.empty()) {
            cout << "No suggestions to reject.\n";
            return;
        }
        int id;
        cout << "Enter the Suggestion ID to reject: ";
        cin >> id;
        auto it = find_if(suggestions.begin(), suggestions.end(), [id](const Movie& m) { return m.id == id; });
        if (it != suggestions.end()) {
            suggestions.erase(it);
            saveSuggestions();
            cout << "Suggestion rejected and removed.\n";
        }
        else {
            cout << "Suggestion ID not found.\n";
        }
    }

    void MovieManager::addDefaultMovies() {
        movies.push_back({ nextMovieId++, "Avengers: Endgame", "Action", "English", "2019-04-26" });
        movies.push_back({ nextMovieId++, "Spirited Away", "Animation", "Japanese", "2001-07-20" });
        movies.push_back({ nextMovieId++, "3 Idiots", "Comedy/Drama", "Hindi", "2009-12-25" });
    }


    set<string> MovieManager::getBookedSeatsForProjection(const string& movieTitle,
        const string& time,
        const string& format)
    {
        set<string> bookedSeats;
        ifstream in(purchaseFile);
        if (!in.is_open()) return bookedSeats;

        string line;
        while (getline(in, line)) {
            stringstream ss(line);
            string buyer, title, projTime, projFormat, seatsLeftStr, seatCode;

            getline(ss, buyer, '|');
            getline(ss, title, '|');
            getline(ss, projTime, '|');
            getline(ss, projFormat, '|');
            getline(ss, seatsLeftStr, '|');
            getline(ss, seatCode);

            if (title == movieTitle && projTime == time && projFormat == format) {
                bookedSeats.insert(seatCode);
            }
        }
        return bookedSeats;
    }


    string MovieManager::chooseSeat(int seatsCount, const set<string>& bookedSeats) {
        const int seatsPerRow = 10;
        int totalRows = (seatsCount + seatsPerRow - 1) / seatsPerRow;

        system("cls");

        cout << "\nSCREEN\n\n";

        for (int row = 0; row < totalRows; ++row) {
            char rowLetter = 'A' + row;
            int seatsThisRow = (row == totalRows - 1) ? (seatsCount - row * seatsPerRow) : seatsPerRow;

            for (int seatNum = 1; seatNum <= seatsThisRow; ++seatNum) {
                string seatCode = string(1, rowLetter) + to_string(seatNum);

                if (bookedSeats.find(seatCode) != bookedSeats.end()) {
                    cout << "[\x1B[31m" << seatCode << "\x1B[0m]";
                }
                else {
                    cout << "[" << seatCode << "]";
                }
            }
            cout << "\n";
        }

        string selectedSeat;
        while (true) {
            cout << "Choose your seat (e.g., B5): ";
            cin >> selectedSeat;

            if (selectedSeat.size() < 2 || selectedSeat.size() > 3) {
                cout << "Invalid format.\n";
                continue;
            }

            selectedSeat[0] = toupper(selectedSeat[0]);
            int seatNum = 0;
            try {
                seatNum = stoi(selectedSeat.substr(1));
            }
            catch (...) {
                cout << "Invalid seat number.\n";
                continue;
            }

            int rowIndex = selectedSeat[0] - 'A';
            if (rowIndex < 0 || rowIndex >= totalRows || seatNum < 1 || seatNum > seatsPerRow) {
                cout << "Seat does not exist.\n";
                continue;
            }
            if (rowIndex == totalRows - 1) {
                int lastRowSeats = seatsCount - rowIndex * seatsPerRow;
                if (seatNum > lastRowSeats) {
                    cout << "Seat does not exist.\n";
                    continue;
                }
            }

            if (bookedSeats.find(selectedSeat) != bookedSeats.end()) {
                cout << "Seat already booked, choose another.\n";
                continue;
            }

            break;
        }

        return selectedSeat;
    }

    


