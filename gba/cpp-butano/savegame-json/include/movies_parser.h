#ifndef MOVIES_PARSER_H
#define MOVIES_PARSER_H

#include "abstract_stackable_parser_handler.h"
#include "savegame.h"
#include "bn_log.h"
#include "rapidjson_inc_no_warns.h"
#include "movies.h"
#include "parsers_types.h"

struct MoviesParserHandler : public TAbstractStackableParserHandler<Movies *>
{
    MoviesParserHandler() : TAbstractStackableParserHandler<Movies *>()
    {
        BN_LOG("Creating ", this->parser_name());
        this->parse_result = new Movies();
    }

    ~MoviesParserHandler()
    {
        if (destruct_result)
        {
            Movies *r = get_result();
            int last_index = r->size() - 1;
            for (int i = last_index; i >= 0; i--)
            {
                Movie *movie = r->at(i);
                delete movie;
            }
            delete r;
        }
    }

    inline char const *parser_name() override
    {
        return "MoviesParserHandler";
    }

    inline bool is_array() override
    {
        return true;
    }

    void process_start_object() override
    {
        subparser_type_id = parsers_types::MOVIE;
        _logToken("start object {");
    }

    // void process_start_array() override
    // {
    //     if (tokens_count == 1)
    //     {
    //         set_start_level_from_current();
    //         _logToken("start array [, update start level");
    //         return;
    //     }
    //     _logToken("start array [");
    // }

    bool subparser_finished_returns_if_destruct_result(std::any subparser_result) override
    {
        Movies *r = get_result();

        switch (subparser_type_id)
        {
        case parsers_types::MOVIE:
        {
            Movie *m = std::any_cast<Movie *>(subparser_result);
            BN_LOG("Adding movie with id ", m->chars_wrapper_id.get_chars(), " to movies");
            r->push_back(m);
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

#endif // MOVIES_PARSER_H