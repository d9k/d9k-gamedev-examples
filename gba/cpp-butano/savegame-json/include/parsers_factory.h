#ifndef PARSERS_FACTORY_H
#define PARSERS_FACTORY_H

#include "abstract_stackable_parser_handler.h"
#include "parsers_types.h"
#include "bn_assert.h"
#include "bn_string.h"
#include "savegame_parser.h"
#include "movie_parser.h"
#include "movies_parser.h"
#include "movie_title_text_parser_deprecated.h"
#include "movie_plot_parser.h"

using namespace parsers_types;

AbstractStackableParserHandler* create_parser_handler_from_type_id(int parserTypeId) {
    switch (parserTypeId)
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
    case MOVIE_TITLETEXT_DEPRECATED: {
        return (AbstractStackableParserHandler*) new MovieTitleTextParserDeprecated();
        break;
    }
    case MOVIE_PLOT: {
        return (AbstractStackableParserHandler*) new MoviePlotParser();
        break;
    }
    default:
        bn::string<64> errorMsg = "No parser found for id ";
        errorMsg += bn::to_string<16>(parserTypeId);
        BN_ASSERT(false, errorMsg);
        break;
    }
    exit(1);
}

#endif // PARSERS_FACTORY_H