#ifndef SAVEGAME_PARSER_HANDLER_H
#define SAVEGAME_PARSER_HANDLER_H

#include "abstract_stackable_parser_handler.h"
#include "savegame.h"
#include "bn_log.h"
#include "movies_parser_handler.h"
#include "rapidjson/reader.h"

typedef TAbstractStackableParserHandler<SaveGame> AbstractSaveGameParserHandler;

struct SaveGameParserHandler : public AbstractSaveGameParserHandler
{
    static constexpr const char * KEY_MOVIES = "movies";
    // char *parser_name = "SaveGameParserHandler";

    SaveGameParserHandler() {
        BN_LOG("Creating SaveGameParserHandler, parser name: ", this->parser_name());
    }

    char* parser_name() override {
        return "SaveGameParserHandler";
    }

    bool process_key(const char *str, rapidjson::SizeType length, bool copy) override {
        // BN_LOG("SaveGameParserHandler key parse begin");

        if (strcmp(KEY_MOVIES, current_key) == 0) {
            // this->last_parse_event = ParserEvent::IMMERSE_TO_SUBPARSER;
            subparser = (AbstractStackableParserHandler*)(new MoviesParserHandler());
        }

        // return _logTokenString(str, length, copy, "key");
        // if (this->current_key) {
        // BN_LOG(this->parser_name(), ": custom process key: ", this->current_key);

        BN_LOG(
            parser_name(),
            ": custom process key: ",
            current_key,
            subparser == NULL ? "" : " DIVING! "
        );
        return true;
    }
};


#endif // SAVEGAME_PARSER_HANDLER_H