#ifndef PARSERS_FACTORY_H
#define PARSERS_FACTORY_H

#include "abstract_stackable_parser_handler.h"
#include "parsers_ids.h"
#include "bn_assert.h"
#include "bn_string.h"
#include "savegame_parser_handler.h"
#include "movie_parser_handler.h"
#include "movies_parser_handler.h"

using namespace parsers_ids;

AbstractStackableParserHandler* create_parser_handler_from_id(int parserId) {
    switch (parserId)
    {
    case SAVEGAME: {
        return (AbstractStackableParserHandler*) new SaveGameParserHandler();
        break;
    }
    case MOVIE: {
        return (AbstractStackableParserHandler*) new MovieParserHandler();
        break;
    }
    case MOVIES: {
        return (AbstractStackableParserHandler*) new MoviesParserHandler();
        break;
    }
    default:
        bn::string<64> errorMsg = "No parser found for id ";
        errorMsg += bn::to_string<16>(parserId);
        BN_ASSERT(false, errorMsg);
        break;
    }
}

#endif // PARSERS_FACTORY_H