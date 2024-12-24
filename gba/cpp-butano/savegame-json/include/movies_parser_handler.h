#ifndef MOVIES_PARSER_HANDLER_H
#define MOVIES_PARSER_HANDLER_H

#include "abstract_stackable_parser_handler.h"
#include "savegame.h"
#include "bn_log.h"
#include "rapidjson/reader.h"
#include "movies.h"
#include "parser_event.h"

struct MoviesParserHandler : public TAbstractStackableParserHandler<Movies>
{
    static constexpr const char *KEY_DEPRECATED_TITLE_TEXT = "titleText";
    static constexpr const char *KEY_TITLE = "title";
    static constexpr const char *KEY_ID = "id";

    MoviesParserHandler() {
        BN_LOG("Creating MoviesParserHandler, parser name: ", this->parser_name());
    }

    char *parser_name() override
    {
        return "MoviesParserHandler";
    }

    bool process_key(const char *str, rapidjson::SizeType length, bool copy) override
    {
        // bool match = false;
        if (strcmp(KEY_DEPRECATED_TITLE_TEXT, current_key) == 0)
        {
            // match = true;
            // this->last_parse_event = ParserEvent::IMMERSE_TO_SUBPARSER;
            // this->subparser = new
        }
        // return _logTokenString(str, length, copy, "key");
        // if (this->current_key) {
        // BN_LOG(this->parser_name(), ": custom process key: ", this->current_key);
        BN_LOG(
            parser_name(),
            ": custom process key: ",
            current_key//,
            // this->last_parse_event == ParserEvent::IMMERSE_TO_SUBPARSER ? " MATCH! " : ""
        );
        return true;
    }
};

#endif // MOVIES_PARSER_HANDLER_H