#ifndef MOVIES_PARSER_H
#define MOVIES_PARSER_H

#include "abstract_stackable_parser_handler.h"
#include "savegame.h"
#include "bn_log.h"
#include "rapidjson/reader.h"
#include "movies.h"
#include "parsers_types.h"

struct MoviesParserHandler : public TAbstractStackableParserHandler<Movies *>
{
    MoviesParserHandler() : TAbstractStackableParserHandler<Movies *>()
    {
        BN_LOG("Creating ", this->parser_name());
        this->result = new Movies();
    }

    ~MoviesParserHandler()
    {
        if (destruct_result) {
            Movies *r = get_result();
            delete r;
        }
    }

    char *parser_name() override
    {
        return "MoviesParserHandler";
    }

    bool process_start_object() override
    {
        subparser_type_id = parsers_types::MOVIE;
        return _logToken("start object {");
    }

    bool process_start_array() override
    {
        this->set_start_level_from_current();
        return _logToken("start array [");
    }

    bool subparser_finished(std::any subparser_result) override
    {
        Movies *r = get_result();

        switch (subparser_type_id)
        {
            case parsers_types::MOVIE: {
                Movie *m = std::any_cast<Movie *>(subparser_result);
                BN_LOG("Adding movie with id ", (*m).id, " to movies");
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

    // bool process_end_array(rapidjson::SizeType elementCount) override
    // {
    //     this->finished = true;
    //     char objectText[32];
    //     sprintf(objectText, "] end array with %d elements", elementCount);
    //     return _logToken(objectText);
    // }
};

#endif // MOVIES_PARSER_H