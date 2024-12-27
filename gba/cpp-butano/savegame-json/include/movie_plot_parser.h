#ifndef MOVIE_PLOT_PARSER_H
#define MOVIE_PLOT_PARSER_H

#include "bn_log.h"
#include "abstract_stackable_parser_handler.h"
#include "chars_copy.h"

struct MoviePlotParser : public TAbstractStackableParserHandler<char *>
{
    MoviePlotParser() : TAbstractStackableParserHandler<char *>()
    {
        BN_LOG("Creating ", this->parser_name());
        BN_LOG(this->parser_name(), ": ", 100);
        // set_result("[no title parsed]");
        result = chars_copy("");
        BN_LOG(this->parser_name(), ": ", 200);
    }

    inline char *parser_name() override
    {
        return "MoviePlotParser";
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
        char *t = get_result();
        delete t;
        result = chars_copy(str);
        BN_LOG("Copying plot text");
        return _logTokenString(str, length, copy);
    }

    ~MoviePlotParser()
    {
        if (destruct_result)
        {
            char *t = get_result();
            delete t;
        }
    }
};

#endif // MOVIE_PLOT_PARSER_H