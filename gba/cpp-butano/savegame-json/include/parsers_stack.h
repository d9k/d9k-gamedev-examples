#ifndef PARSERS_STACK_H
#define PARSERS_STACK_H

// #include <list>
#include <vector>
#include <any>
#include "abstract_stackable_parser_handler.h"
#include "rapidjson/reader.h"

/**
 * For iterative reading
 *
 * https://rapidjson.org/md_doc_sax.html
 */
class ParsersStack
{
public:
    std::vector<AbstractStackableParserHandler*> _parsers_handlers;
    AbstractStackableParserHandler *_current_parser_handler;
    rapidjson::Reader *_reader;
    rapidjson::StringStream *_input_stream;

    ParsersStack(AbstractStackableParserHandler *root_parser_handler, rapidjson::Reader *reader, rapidjson::StringStream *input_stream)
    {
        _parsers_handlers = {};
        add(root_parser_handler);
        _reader = reader;
        _reader->IterativeParseInit();
        _input_stream = input_stream;
    }

    void add(AbstractStackableParserHandler *new_parser_handler)
    {
        _parsers_handlers.push_back(new_parser_handler);
        _current_parser_handler = new_parser_handler;
        // last_parser->last_parse_event;
        // last_parser->parser_name;
        // last_parser->result;
    }

    bool pop()
    {
        if (_parsers_handlers.size() <= 1) {
            return false;
        }
        _parsers_handlers.pop_back();
        _current_parser_handler = (AbstractStackableParserHandler*)&(_parsers_handlers.back());
        return true;
    }

    bool parse_next_token()
    {
        if (_reader->IterativeParseComplete())
        {
            return false;
        }
        _reader->IterativeParseNext<rapidjson::kParseDefaultFlags>(*_input_stream, *_current_parser_handler);
        return true;
    }
};

#endif // PARSERS_STACK_H