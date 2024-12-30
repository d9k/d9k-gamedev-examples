#ifndef MOVIE_SERIALIZER_H
#define MOVIE_SERIALIZER_H

#include "rapidjson_inc_no_warns.h"

#include "movie.h"
#include "movie_parser_keys.h"

void serialize_movie(
    rapidjson::Writer<rapidjson::StringBuffer> *writer,
    Movie *movie
) {
    writer->StartObject();

    writer->Key(movie_parser_keys::KEY_ID);
    writer->String(movie->id.get_chars());

    writer->Key(movie_parser_keys::KEY_TITLE);
    writer->String(movie->title.get_chars());

    writer->Key(movie_parser_keys::KEY_YEAR);
    writer->Int(movie->year);

    writer->Key(movie_parser_keys::KEY_PLOT_TEXT);
    writer->String(movie->plot_text.get_chars());

    writer->EndObject();
}

#endif // MOVIE_SERIALIZER_H