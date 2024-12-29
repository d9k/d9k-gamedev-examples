#ifndef SAVEGAME_SERIALIZER_H
#define SAVEGAME_SERIALIZER_H

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "savegame.h"
#include "savegame_parser_keys.h"
#include "movie_serializer.h"

using namespace savegame_parser_keys;

void serialize_savegame(
    rapidjson::Writer<rapidjson::StringBuffer> *writer,
    SaveGame *saveGame
) {
    writer->StartObject();

    writer->Key(KEY_SELECTED_MOVIE_ID);

    writer->String(saveGame->selected_movie_id);

    writer->Key(KEY_LOADS_COUNT);

    writer->Int(saveGame->loads_count);

    writer->Key(KEY_MOVIES);

    writer->StartArray();

    for (int i = 0; i < saveGame->movies.size(); i++)
    {
        Movie *movie = saveGame->movies[i];
        serialize_movie(writer, movie);
    }

    writer->EndArray();

    writer->EndObject();
}

#endif // SAVEGAME_SERIALIZER_H