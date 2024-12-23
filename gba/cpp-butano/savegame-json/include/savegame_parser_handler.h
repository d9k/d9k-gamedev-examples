#ifndef SAVEGAME_PARSER_HANDLER_H
#define SAVEGAME_PARSER_HANDLER_H

#include "abstract_stackable_parser_handler.h"
#include "savegame.h"
#include "bn_log.h"

typedef TAbstractStackableParserHandler<SaveGame> AbstractSaveGameParserHandler;

namespace save_game_parser_handler {
}

struct SaveGameParserHandler : public AbstractSaveGameParserHandler
{
    static constexpr const char * KEY_MOVIES = "movies";
    // char *parser_name = "SaveGameParserHandler";

    SaveGameParserHandler() {
        BN_LOG("Creating SaveGameParserHandler", this->parser_name());
    }

    char* parser_name() override {
        return "SaveGameParserHandler";
    }

    bool process_key(const char *str, rapidjson::SizeType length, bool copy) override {
        bool match = false;
        if (strcmp(KEY_MOVIES, current_key) == 0) {
            match = true;
        }
        // return _logTokenString(str, length, copy, "key");
        // if (this->current_key) {
        // BN_LOG(this->parser_name(), ": custom process key: ", this->current_key);
        BN_LOG(parser_name(), ": custom process key: ", current_key, match ? " MATCH! " : "");
        // }
        return true;
    }
};


#endif // SAVEGAME_PARSER_HANDLER_H