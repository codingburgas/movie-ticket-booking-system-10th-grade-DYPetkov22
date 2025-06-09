#include "MovieManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

MovieManager::MovieManager() {
    loadMoviesFromFile();
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
    cout << "Enter movie ID to delete: ";
    cin >> id;
    auto it = remove_if(movies.begin(), movies.end(), [id](const Movie& m) { return m.id == id; });
    if (it != movies.end()) {
        movies.erase(it, movies.end());
        saveMoviesToFile();
        cout << "Movie deleted successfully.\n";
    }
    else {
        cout << "Movie ID not found.\n";
    }
}

void MovieManager::viewMovies() {
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

void MovieManager::addDefaultMovies() {
    movies.push_back({ nextMovieId++, "Avengers: Endgame", "Action", "English", "2019-04-26" });
    movies.push_back({ nextMovieId++, "Spirited Away", "Animation", "Japanese", "2001-07-20" });
    movies.push_back({ nextMovieId++, "3 Idiots", "Comedy/Drama", "Hindi", "2009-12-25" });
}

void MovieManager::displayMoviesForUser() {
    cout << "\n--- Available Movies ---\n";
    viewMovies();
}

bool MovieManager::chooseMovieToWatch() {
    if (movies.empty()) {
        cout << "No movies available to watch.\n";
        return false;
    }
    viewMovies();
    cout << "Enter the ID of the movie you want to watch: ";
    int id;
    cin >> id;
    for (const auto& m : movies) {
        if (m.id == id) {
            cout << "\nNow playing: " << m.title << " (" << m.genre << ", " << m.language << ", " << m.releaseDate << ")\n";
            cout << "Enjoy your movie!\n";
            return true;
        }
    }
    cout << "Invalid movie ID.\n";
    return false;
}

// ----------- SUGGESTION SYSTEM -----------

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

void MovieManager::saveSuggestions() {
    ofstream out(suggestionFile);
    for (const auto& m : suggestions) {
        out << m.id << "|" << m.title << "|" << m.genre << "|" << m.language << "|" << m.releaseDate << "\n";
    }
}

void MovieManager::suggestMovie() {
    Movie m;
    m.id = nextSuggestionId++;
    cin.ignore();
    cout << "Suggest a new movie!" << endl;
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
            << "Language: " << m.language << "\n"
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
