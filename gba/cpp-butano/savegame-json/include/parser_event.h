#ifndef PARSER_EVENT_H
#define PARSER_EVENT_H

enum class ParserEvent {
   WAIT_NEXT_TOKEN = 0,
   IMMERSE_TO_SUBPARSER = 1,
   EVENT_PARSE_FINISHED = 2,
};

#endif // PARSER_EVENT_H