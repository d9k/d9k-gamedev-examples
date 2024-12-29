#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <vector>
#include "movies.h"
#include "chars_copy.h"

class SaveGame {
public:
    char* selected_movie_id;
    int loads_count = 0;

    Movies movies = {};

    SaveGame() {
        // selected_movie_id = chars_copy("_TEST_");
        selected_movie_id = "";
    }

    ~SaveGame() {
        delete[] selected_movie_id;
    }

    void set_selected_movie_id(char *v) {
        delete[] selected_movie_id;
        selected_movie_id = chars_copy(v);
    }
};

#endif // SAVEGAME_H
