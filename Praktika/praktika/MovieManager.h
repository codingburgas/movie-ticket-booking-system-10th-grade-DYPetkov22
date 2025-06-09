#pragma once

#include <vector>
#include <string>
#include "Movie.h"

class MovieManager {
public:
    MovieManager();
    void addMovie();
    void deleteMovie();
    void viewMovies();
    void saveMoviesToFile();
    void loadMoviesFromFile();
    void displayMoviesForUser();
    bool chooseMovieToWatch();

    void suggestMovie();
    void loadSuggestions();
    void saveSuggestions();
    void viewSuggestions();
    void approveSuggestion();
    void rejectSuggestion();

private:
    std::vector<Movie> movies;
    int nextMovieId = 1;
    const std::string movieFile = "movies.txt";

    std::vector<Movie> suggestions;
    int nextSuggestionId = 1;
    const std::string suggestionFile = "suggestions.txt";

    void addDefaultMovies();
};