#ifndef MOVIE_PARSER_H
#define MOVIE_PARSER_H

#include <any>
#include "abstract_stackable_parser_handler.h"
#include "savegame.h"
#include "bn_log.h"
#include "rapidjson/reader.h"
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

    inline char *parser_name() override
    {
        return "MovieParserHandler";
    }

    void process_start_object() override
    {
        if (tokens_count == 1)
        {
            set_start_level_from_current();
            _logToken("start object {, update start level");
            return;
        }
        _logToken("start object {");
    }

    void process_uint(int u) override
    {
        if (key_is(KEY_YEAR))
        {
            Movie *r = get_result();
            r->year = u;
        }
        return _logToken(u, "int");
    }

    void process_string(const char *str, rapidjson::SizeType length, bool copy) override
    {
        if (this->get_inside_json_stack_level() == this->start_level)
        {
            if (key_is(KEY_ID))
            {
                Movie *r = get_result();
                r->set_id(str);
                BN_LOG(this->parser_name(), ": check result id: ", r->id);
            }
        }

        _logTokenString(str, length, copy);
        return;
    }

    void process_key(const char *str, rapidjson::SizeType length, bool copy) override
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

    bool subparser_finished(std::any subparser_result) override
    {
        Movie *m = get_result();

        switch (subparser_type_id)
        {
        case parsers_types::MOVIE_TITLETEXT_DEPRECATED:
        {
            char *title = std::any_cast<char *>(subparser_result);
            BN_LOG("Adding title ", title, " to movie object");
            m->set_title(title);
            return true;
            break;
        }
        case parsers_types::MOVIE_PLOT:
        {
            char *plotText = std::any_cast<char *>(subparser_result);
            BN_LOG("Adding plot text ", plotText, " to movie object");
            m->set_plot_text(plotText);
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