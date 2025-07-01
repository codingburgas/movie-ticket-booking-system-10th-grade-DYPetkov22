#pragma once

#include <vector>
#include <string>
#include "Movie.h"
#include <set>

struct Projection {
    int movieId;
    std::string time;
    std::string format;
    int availableSeats;
};

class MovieManager {
public:
    MovieManager();

    void addMovie();
    void deleteMovie();
    void viewMovies();
    void saveMoviesToFile();
    void loadMoviesFromFile();

    void displayMoviesForUser();
    bool chooseMovieToWatch(const std::string& username);

    void suggestMovie();
    void loadSuggestions();
    void saveSuggestions();
    void viewSuggestions();
    void approveSuggestion();
    void rejectSuggestion();
    std::string chooseSeat(int seatsCount, const std::set<std::string>& bookedSeats);
    std::set<std::string> getBookedSeatsForProjection(const std::string& movieTitle,
        const std::string& time,
        const std::string& format);

    void viewAndDeleteMyPurchases(const std::string& username);

    void addProjection();
    void viewProjections();
    void saveProjectionsToFile();
    void loadProjectionsFromFile();

    void viewPurchasesForMovie();

private:
    std::vector<Movie> movies;
    int nextMovieId = 1;
    const std::string movieFile = "movies.txt";

    std::vector<Movie> suggestions;
    int nextSuggestionId = 1;
    const std::string suggestionFile = "suggestions.txt";

    std::vector<Projection> projections;
    const std::string projectionFile = "projections.txt";

    void addDefaultMovies();

    const std::string purchaseFile = "purchases.txt";      
    void logPurchase(const std::string& buyer, const Projection& proj, const Movie& mov, const std::string& seatCode);

    

};
