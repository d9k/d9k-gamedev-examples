#ifndef MOVIE_H
#define MOVIE_H

#include <string>
#include "bn_core.h"
#include "bn_string.h"

class MovieReleaseDate {
    uint8_t t;
};

class Movie {
public:
    // int test;
    // char *id = (char*)"";
    // char *title = (char*)"";
    std::string title;
    std::string id;

    ~Movie() {
        // delete[] title;
        // delete title;
        // delete id;
    }
// public:
//     Movie()
//     {
//     }
};

#endif