#ifndef MOVIES_PARSER_H
#define MOVIES_PARSER_H

#include "abstract_stackable_parser_handler.h"
#include "savegame.h"
#include "bn_log.h"
#include "rapidjson/reader.h"
#include "movies.h"
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

    bool process_start_array() override
    {
        this->set_start_level_from_current();
        return _logToken("start array [");
    }

    // bool process_end_array(rapidjson::SizeType elementCount) override
    // {
    //     this->finished = true;
    //     char objectText[32];
    //     sprintf(objectText, "] end array with %d elements", elementCount);
    //     return _logToken(objectText);
    // }
};

#endif // MOVIES_PARSER_H