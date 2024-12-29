#ifndef MOVIE_SERIALIZER_H
#define MOVIE_SERIALIZER_H

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "movie.h"
#include "movie_parser_keys.h"

void serialize_movie(
    rapidjson::Writer<rapidjson::StringBuffer> *writer,
    Movie *movie
) {
    writer->StartObject();

    writer->Key(movie_parser_keys::KEY_ID);
    writer->String(movie->id);

    writer->Key(movie_parser_keys::KEY_TITLE);
    writer->String(movie->title);

    writer->Key(movie_parser_keys::KEY_YEAR);
    writer->Int(movie->year);

    writer->Key(movie_parser_keys::KEY_PLOT_TEXT);
    writer->String(movie->plotText);

    writer->EndObject();
}

#endif // MOVIE_SERIALIZER_H