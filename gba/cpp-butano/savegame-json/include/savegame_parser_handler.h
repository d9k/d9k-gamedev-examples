#ifndef SAVEGAME_PARSER_HANDLER_H
#define SAVEGAME_PARSER_HANDLER_H

#include "abstract_stackable_parser_handler.h"
#include "savegame.h"

typedef TAbstractStackableParserHandler<SaveGame> AbstractSaveGameParserHandler;

struct SaveGameParserHandler : public AbstractSaveGameParserHandler
{
    // char *parser_name = "SaveGameParserHandler";

    SaveGameParserHandler() {
        BN_LOG("Creating SaveGameParserHandler", this->parser_name());
    }

    char* parser_name() override;
};

char* SaveGameParserHandler::parser_name () {
    return "SaveGameParserHandler";
}


#endif // SAVEGAME_PARSER_HANDLER_H