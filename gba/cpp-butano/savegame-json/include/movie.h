#ifndef MOVIE_H
#define MOVIE_H

#include <string>
#include "bn_core.h"
#include "bn_string.h"
#include "chars_copy.h"

class MovieReleaseDate {
    uint8_t t;
};

class Movie {
public:
    // int test;
    // char *id = (char*)"";
    // char *title = (char*)"";
    // bn::string_view *id;
    // bn::string_view *title;
    char* id;
    char* title;

    Movie() {
        id = chars_copy("[no id]");
        title = chars_copy("[no title]");
        // id = new bn::string_view("[no id]");
        // title = new bn::string_view("[no title]");
        BN_LOG("### MOVIE INIT ###", id, ",", title, ", ", *id, ", ", *title);
    }

    ~Movie() {
        delete title;
        delete id;
        // delete[] title;
        // delete title;
        // delete id;
    }

    void set_id(const char* v) {
        delete id;
        // id = new bn::string_view(v);
        id = chars_copy(v);
    }
// public:
//     Movie()
//     {
//     }
};

#endif