#ifndef SAVEGAME_PARSER_H
#define SAVEGAME_PARSER_H

#include <any>
#include "abstract_stackable_parser_handler.h"
#include "savegame.h"
#include "bn_assert.h"
#include "bn_string.h"
#include "bn_log.h"
#include "rapidjson/reader.h"
#include "savegame_parser_keys.h"
#include "parsers_types.h"
#include "movies.h"
#include "savegame.h"

using namespace savegame_parser_keys;

struct SaveGameParserHandler : public TAbstractStackableParserHandler<SaveGame *>
{
    SaveGameParserHandler()
    {
        BN_LOG("Creating SaveGameParserHandler, parser name: ", this->parser_name());
        this->parse_result = new SaveGame();
    }

    ~SaveGameParserHandler()
    {
        if (destruct_result)
        {
            SaveGame *r = get_result();
            delete r;
        }
    }

    inline char *parser_name() override
    {
        return "SaveGameParserHandler";
    }

    void process_key(const char *str, rapidjson::SizeType length, bool copy) override
    {
        // if (current_key == KEY_MOVIES) {
        if (key_is(KEY_MOVIES))
        {
            subparser_type_id = parsers_types::MOVIES;
        }

        BN_LOG(
            parser_name(),
            ": custom process key: ",
            current_key,
            ", subparser_type:",
            subparser_type_id);
    }

    virtual void process_start_object()
    {
        if (tokens_count == 1)
        {
            set_start_level_from_current();
            _logToken("start object {, update start level");
            return;
        }
        _logToken("start object {");
    }

    bool subparser_finished_returns_if_destruct_result(std::any subparser_result) override
    {
        SaveGame *r = get_result();

        switch (subparser_type_id)
        {
        case parsers_types::MOVIES:
        {
            Movies *m = std::any_cast<Movies *>(subparser_result);
            BN_LOG("Adding movies with size ", m->size(), " to save game object");
            r->movies = *m;
            return false;
            break;
        }
        default:
            this->error_no_subparser_found();
            break;
        }
        return true;
    }
};

#endif // SAVEGAME_PARSER_H