#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <vector>
#include "movies.h"

class SaveGame {
public:
    char* selected_movie_id;
    int loads_count = 0;

    Movies movies = {};

    SaveGame() {
    }

    ~SaveGame() {
        delete[] selected_movie_id;
    }
};

#endif // SAVEGAME_H
