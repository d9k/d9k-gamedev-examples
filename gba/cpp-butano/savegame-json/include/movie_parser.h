#ifndef MOVIE_PARSER_H
#define MOVIE_PARSER_H

#include <any>
#include "abstract_stackable_parser_handler.h"
#include "savegame.h"
#include "bn_log.h"
#include "rapidjson/reader.h"
#include "movie.h"
#include "movie_parser_keys.h"

using namespace movie_parser_keys;

struct MovieParserHandler : public TAbstractStackableParserHandler<Movie *>
{
    MovieParserHandler() : TAbstractStackableParserHandler<Movie *>()
    {
        BN_LOG("Creating ", this->parser_name());
        this->result = new Movie();
    }

    ~MovieParserHandler()
    {
        if (destruct_result) {
            Movie *r = get_result();
            delete r;
        }
    }

    char *parser_name() override
    {
        return "MovieParserHandler";
    }

    bool process_string(const char *str, rapidjson::SizeType length, bool copy) override
    {
        Movie *r = get_result();
        if (this->get_inside_stack_level() == this->start_level) {
            if (key_is(KEY_ID))
            {
                r->set_id(str);
                BN_LOG(this->parser_name(), ": check result id: ", r->id);
            }
        }

        return _logTokenString(str, length, copy);
    }

    bool process_key(const char *str, rapidjson::SizeType length, bool copy) override
    {
        if (key_is(KEY_TITLE_TEXT_DEPRECATED))
        {
        }

        BN_LOG(
            parser_name(),
            ": custom process key: ",
            current_key);
        return true;
    }
};

#endif // MOVIE_PARSER_H