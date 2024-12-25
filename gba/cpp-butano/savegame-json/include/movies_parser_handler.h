#ifndef MOVIES_PARSER_HANDLER_H
#define MOVIES_PARSER_HANDLER_H

#include "abstract_stackable_parser_handler.h"
#include "savegame.h"
#include "bn_log.h"
#include "rapidjson/reader.h"
#include "movies.h"
#include "movie_parser_handler.h"

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

    TAbstractStackableParserHandler<std::any> *get_subparser_if_needed() override
    {
        switch (this->subparser_type) {
            case SUBPARSER_TYPE_MOVIE: {
                return (AbstractStackableParserHandler*) new MovieParserHandler();
                break;
            }
            default:
                return NULL;
        }
    }

    bool process_start_object() override {
        subparser_type = SUBPARSER_TYPE_MOVIE;
        return _logToken("start object {");
    }

    bool process_end_array(rapidjson::SizeType elementCount) override {
        this->finished = true;
        char objectText[32];
        sprintf(objectText, "] end array with %d elements", elementCount);
        return _logToken(objectText);
    }
};

#endif // MOVIES_PARSER_HANDLER_H