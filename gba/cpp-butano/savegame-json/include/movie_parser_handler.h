#ifndef MOVIE_PARSER_HANDLER_H
#define MOVIE_PARSER_HANDLER_H

#include <any>
#include "abstract_stackable_parser_handler.h"
#include "savegame.h"
#include "bn_log.h"
#include "rapidjson/reader.h"
#include "movie.h"

struct MovieParserHandler : public TAbstractStackableParserHandler<Movie*>
// struct MovieParserHandler : public TAbstractStackableParserHandler<std::any>
{
    static constexpr const char *KEY_TITLE_TEXT_DEPRECATED = "titleText";
    static constexpr const char *KEY_TITLE = "title";
    static constexpr const char *KEY_ID = "id";

    MovieParserHandler(): TAbstractStackableParserHandler<Movie*>() {
        BN_LOG("Creating ", this->parser_name());
        this->result = new Movie();
    }

    ~MovieParserHandler() {
        delete std::any_cast<Movie>(&result);
    }

    char *parser_name() override
    {
        return "MovieParserHandler";
    }

    // void result_destroy() override
    // {
    //     // delete get_result();
    // }

    // Movie* get_result() override
    // Movie* get_result() {
    //     return std::any_cast<Movie*>(result);
    // }

    bool process_string(const char *str, rapidjson::SizeType length, bool copy) override {
        // Movie * result = get_result();
        // Movie * r = std::any_cast<Movie *>(result);
        // Movie * r = std::any_cast<Movie>(&result);// get_result();
        Movie * r = std::any_cast<Movie*>(result);// get_result();
        if (strcmp(KEY_ID, current_key) == 0) {
            r->set_id(str);
            BN_LOG(this->parser_name(), ": check result: ", *r->id);
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
            current_key
        );
        return true;
    }

    // bool process_end_object(rapidjson::SizeType memberCount) override {
    //     this->finished = true;
    //     char objectText[32];
    //     sprintf(objectText, "!!! } end object with %d members", memberCount);
    //     return _logToken(objectText);
    // }

};

#endif // MOVIE_PARSER_HANDLER_H