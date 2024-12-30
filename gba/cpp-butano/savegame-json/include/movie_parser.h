#ifndef MOVIE_PARSER_H
#define MOVIE_PARSER_H

#include <any>
#include "abstract_stackable_parser_handler.h"
#include "savegame.h"
#include "bn_log.h"
#include "rapidjson_inc_no_warns.h"
#include "movie.h"
#include "movie_parser_keys.h"
#include "parsers_types.h"

using namespace movie_parser_keys;

struct MovieParserHandler : public TAbstractStackableParserHandler<Movie *>
{
    MovieParserHandler() : TAbstractStackableParserHandler<Movie *>()
    {
        BN_LOG("Creating ", this->parser_name());
        this->parse_result = new Movie();
    }

    ~MovieParserHandler()
    {
        if (destruct_result)
        {
            Movie *r = get_result();
            delete r;
        }
    }

    inline const char *parser_name() override
    {
        return "MovieParserHandler";
    }

    inline bool is_object() override
    {
        return true;
    }

    void process_int(int i) override
    {
        if (key_is(KEY_YEAR))
        {
            Movie *r = get_result();
            r->year = i;
        }
        _logToken(i, "int");
    }

    void process_string(const char *str, rapidjson::SizeType length, bool copy) override
    {
        if (this->get_inside_json_stack_level() == this->start_level)
        {
            if (key_is(KEY_ID))
            {
                Movie *r = get_result();
                r->id.set_chars(str);
                // BN_LOG(this->parser_name(), ": check result id: ", r->id.get_chars());
            }
        }

        _logTokenString(str, length, copy);
    }

    void process_key(const char* /* str */, rapidjson::SizeType /* length */, bool /* copy */) override
    {
        if (key_is(KEY_TITLE_TEXT_DEPRECATED))
        {
            this->subparser_type_id = parsers_types::MOVIE_TITLETEXT_DEPRECATED;
        }

        if (key_is(KEY_PLOT_TEXT))
        {
            this->subparser_type_id = parsers_types::MOVIE_PLOT;
        }

        BN_LOG(
            parser_name(),
            ": custom process key: ",
            current_key,
            ", subparser type id:",
            this->subparser_type_id);
    }

    bool subparser_finished_returns_if_destruct_result(std::any subparser_result) override
    {
        Movie *m = get_result();

        switch (subparser_type_id)
        {
        case parsers_types::MOVIE_TITLETEXT_DEPRECATED:
        {
            char *title = std::any_cast<char *>(subparser_result);
            BN_LOG("Adding title ", title, " to movie object");
            m->title.set_chars(title);
            return true;
            break;
        }
        case parsers_types::MOVIE_PLOT:
        {
            char *plotText = std::any_cast<char *>(subparser_result);
            BN_LOG("Adding plot text ", plotText, " to movie object");
            m->plot_text.set_chars(plotText);
            return true;
            break;
        }
        default:
            this->error_no_subparser_found();
            break;
        }
        return true;
    }
};

#endif // MOVIE_PARSER_H