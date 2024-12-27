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
        this->result = new Movie();
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

    virtual bool process_start_object()
    {
        if (tokens_count == 1)
        {
            set_start_level_from_current();
            return _logToken("start object {, update start level");
        }
        return _logToken("start object {");
    }

    bool process_string(const char *str, rapidjson::SizeType length, bool copy) override
    {
        Movie *r = get_result();
        if (this->get_inside_stack_level() == this->start_level)
        {
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
            this->subparser_type_id = parsers_types::MOVIE_TITLETEXT_DEPRECATED;
        }

        BN_LOG(
            parser_name(),
            ": custom process key: ",
            current_key);
        return true;
    }

    bool subparser_finished(std::any subparser_result) override
    {
        Movie *m = get_result();

        switch (subparser_type_id)
        {
            case parsers_types::MOVIE_TITLETEXT_DEPRECATED:  {
                char *title = std::any_cast<char *>(subparser_result);
                BN_LOG("Adding title ", title, " to movie object");
                m->set_title(title);
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

#endif // MOVIE_PARSER_H