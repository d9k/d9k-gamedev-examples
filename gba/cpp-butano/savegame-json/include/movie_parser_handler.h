#ifndef MOVIE_PARSER_HANDLER_H
#define MOVIE_PARSER_HANDLER_H

#include <any>
#include "abstract_stackable_parser_handler.h"
#include "savegame.h"
#include "bn_log.h"
#include "rapidjson/reader.h"
#include "movie.h"

struct MovieParserHandler : public TAbstractStackableParserHandler<Movie *>
{
    static constexpr const char *KEY_TITLE_TEXT_DEPRECATED = "titleText";
    static constexpr const char *KEY_TITLE = "title";
    static constexpr const char *KEY_ID = "id";

    MovieParserHandler() : TAbstractStackableParserHandler<Movie *>()
    {
        BN_LOG("Creating ", this->parser_name());
        this->result = new Movie();
    }

    ~MovieParserHandler()
    {
        Movie *r = get_result();
        delete r;
    }

    char *parser_name() override
    {
        return "MovieParserHandler";
    }

    bool process_string(const char *str, rapidjson::SizeType length, bool copy) override
    {
        Movie *r = get_result();
        if (strcmp(KEY_ID, current_key) == 0)
        {
            r->set_id(str);
            BN_LOG(this->parser_name(), ": check result id: ", *r->id);
        }

        return _logTokenString(str, length, copy);
    }

    bool process_key(const char *str, rapidjson::SizeType length, bool copy) override
    {
        if (strcmp(KEY_TITLE_TEXT_DEPRECATED, current_key) == 0)
        {
        }

        BN_LOG(
            parser_name(),
            ": custom process key: ",
            current_key);
        return true;
    }
};

#endif // MOVIE_PARSER_HANDLER_H