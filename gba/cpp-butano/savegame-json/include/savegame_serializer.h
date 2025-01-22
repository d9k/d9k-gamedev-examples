#ifndef SAVEGAME_SERIALIZER_H
#define SAVEGAME_SERIALIZER_H

#include "rapidjson_inc_no_warns.h"
#include "savegame.h"
#include "savegame_parser_keys.h"
#include "movie_serializer.h"

using namespace savegame_parser_keys;

void serialize_savegame(
    rapidjson::Writer<rapidjson::StringBuffer> *writer,
    SaveGame *saveGame)
{
    writer->StartObject();

    writer->Key(KEY_SELECTED_MOVIE_ID);

    writer->String(saveGame->chars_wrapper_selected_movie_id.get_chars());

    writer->Key(KEY_LOADS_COUNT);

    writer->Int(saveGame->loads_count);

    writer->Key(KEY_MOVIES);

    writer->StartArray();

    for (uint32_t i = 0; i < saveGame->_movies->size(); i++)
    {
        Movie *movie = saveGame->_movies->at(i);
        serialize_movie(writer, movie);
    }

    writer->EndArray();

    writer->EndObject();
}

#endif // SAVEGAME_SERIALIZER_H