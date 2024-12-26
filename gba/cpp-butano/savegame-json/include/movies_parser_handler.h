#ifndef MOVIES_PARSER_HANDLER_H
#define MOVIES_PARSER_HANDLER_H

#include "abstract_stackable_parser_handler.h"
#include "savegame.h"
#include "bn_log.h"
#include "rapidjson/reader.h"
#include "movies.h"
#include "movie_parser_handler.h"
#include "parsers_ids.h"

struct MoviesParserHandler : public TAbstractStackableParserHandler<Movies>
{
    static constexpr const int SUBPARSER_TYPE_MOVIE = 1;

    MoviesParserHandler() {
        BN_LOG("Creating MoviesParserHandler, parser name: ", this->parser_name());
    }

    char *parser_name() override
    {
        return "MoviesParserHandler";
    }

    bool process_start_object() override {
        subparser_type = parsers_ids::MOVIE;
        return _logToken("start object {");
    }
};

#endif // MOVIES_PARSER_HANDLER_H