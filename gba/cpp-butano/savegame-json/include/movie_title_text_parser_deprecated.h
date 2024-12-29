#ifndef MOVIE_TITLE_TEXT_PARSER_DEPRECATED_H
#define MOVIE_TITLE_TEXT_PARSER_DEPRECATED_H

#include "bn_log.h"
#include "abstract_stackable_parser_handler.h"
#include "chars_copy.h"

namespace movie_title_text_parser_keys
{
    constexpr const char *KEY_TEXT = "text";
}

using namespace movie_title_text_parser_keys;

struct MovieTitleTextParserDeprecated : public TAbstractStackableParserHandler<char *>
{
    MovieTitleTextParserDeprecated() : TAbstractStackableParserHandler<char *>()
    {
        BN_LOG("Creating ", this->parser_name());
        BN_LOG(this->parser_name(), ": ", 100);
        // set_result("[no title parsed]");
        parse_result = chars_copy("[no title parsed]");
        BN_LOG(this->parser_name(), ": ", 200);
    }

    inline char const *parser_name() override
    {
        return "MovieTitleTextParserDeprecated";
    }

    ~MovieTitleTextParserDeprecated()
    {
        if (destruct_result)
        {
            char *r = get_result();
            delete[] r;
        }
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

    void process_string(const char *str, rapidjson::SizeType length, bool copy) override
    {
        if (key_is(KEY_TEXT))
        {
            set_result(str);
            BN_LOG(this->parser_name(), ": check result text: ", get_result());
        }

        _logTokenString(str, length, copy);
    }

    void set_result(const char *value)
    {
        // BN_LOG(this->parser_name(), ": set_result: ", 100);
        char *r = get_result();
        // BN_LOG(this->parser_name(), ": set_result: ", 200);
        delete[] r;
        // BN_LOG(this->parser_name(), ": set_result: ", 300);
        parse_result = chars_copy(value);
        // BN_LOG(this->parser_name(), ": set_result: ", 400);
    }
};

#endif // MOVIE_TITLE_TEXT_PARSER_DEPRECATED_H