#ifndef SAVEGAME_PARSER_HANDLER_H
#define SAVEGAME_PARSER_HANDLER_H

#include "abstract_stackable_parser_handler.h"
#include "savegame.h"
#include "bn_log.h"
#include "movies_parser_handler.h"
#include "rapidjson/reader.h"
#include "savegame_parser_keys.h"

typedef TAbstractStackableParserHandler<SaveGame> AbstractSaveGameParserHandler;

using namespace savegame_parser_keys;

struct SaveGameParserHandler : public AbstractSaveGameParserHandler
{
    static constexpr const int SUBPARSER_TYPE_MOVIES = 1;

    SaveGameParserHandler() {
        BN_LOG("Creating SaveGameParserHandler, parser name: ", this->parser_name());
    }

    char* parser_name() override {
        return "SaveGameParserHandler";
    }

    TAbstractStackableParserHandler<std::any> *get_subparser_if_needed() override
    {
        switch (this->subparser_type) {
            case SUBPARSER_TYPE_MOVIES: {
                return (AbstractStackableParserHandler*) new MoviesParserHandler();
                break;
            }
            default:
                return NULL;
        }
    }

    bool process_key(const char *str, rapidjson::SizeType length, bool copy) override {
        if (strcmp(KEY_MOVIES, current_key) == 0) {
            subparser_type = SUBPARSER_TYPE_MOVIES;
        }

        BN_LOG(
            parser_name(),
            ": custom process key: ",
            current_key,
            ", subparser_type:",
            subparser_type
        );
        return true;
    }
};


#endif // SAVEGAME_PARSER_HANDLER_H