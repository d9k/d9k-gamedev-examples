#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <vector>
#include "movies.h"
#include "chars_copy.h"

class SaveGame {
public:
    CharsPointerCopyWrapper selected_movie_id;
    int loads_count = 0;

    Movies movies = {};
};

#endif // SAVEGAME_H
