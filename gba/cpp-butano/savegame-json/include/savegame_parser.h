#ifndef SAVEGAME_PARSER_H
#define SAVEGAME_PARSER_H

#include "abstract_stackable_parser_handler.h"
#include "savegame.h"
#include "bn_log.h"
#include "rapidjson/reader.h"
#include "savegame_parser_keys.h"
#include "parsers_ids.h"

using namespace savegame_parser_keys;

struct SaveGameParserHandler : public TAbstractStackableParserHandler<SaveGame>
{
    SaveGameParserHandler() {
        BN_LOG("Creating SaveGameParserHandler, parser name: ", this->parser_name());
    }

    char* parser_name() override {
        return "SaveGameParserHandler";
    }

    bool process_key(const char *str, rapidjson::SizeType length, bool copy) override {
        // if (current_key == KEY_MOVIES) {
        if (key_is(KEY_MOVIES)) {
            subparser_type = parsers_ids::MOVIES;
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

    virtual bool process_start_object()
    {
        this->set_start_level_from_current();
        return _logToken("start object {");
    }
};


#endif // SAVEGAME_PARSER_H