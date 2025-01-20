#ifndef SAVEGAME_PARSER_H
#define SAVEGAME_PARSER_H

#include <any>
#include "abstract_stackable_parser_handler.h"
#include "savegame.h"
#include "bn_assert.h"
#include "bn_string.h"
#include "bn_log.h"
#include "rapidjson_inc_no_warns.h"
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

    SaveGame *get_result() override
    {
        SaveGame *result = std::any_cast<SaveGame *>(parse_result);
        result->validate();
        return result;
    }

    inline char const *parser_name() override
    {
        return "SaveGameParserHandler";
    }

    inline bool is_object() override
    {
        return true;
    }

    virtual void process_string(const char *str, rapidjson::SizeType length, bool copy)
    {
        if (key_is(KEY_SELECTED_MOVIE_ID))
        {
            SaveGame *r = get_result();
            r->chars_wrapper_selected_movie_id.set_chars(chars_copy(str));
        }
        _logTokenString(str, length, copy);
    }

    void process_int(int i) override
    {
        if (key_is(KEY_LOADS_COUNT))
        {
            SaveGame *r = get_result();
            r->loads_count = i;
        }
        _logToken(i, "int");
    }

    void process_key(const char * /* str */, rapidjson::SizeType /* length */, bool /* copy */) override
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