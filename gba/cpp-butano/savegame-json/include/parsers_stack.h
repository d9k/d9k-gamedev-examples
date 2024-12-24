#ifndef PARSERS_STACK_H
#define PARSERS_STACK_H

// #include <list>
#include <vector>
#include <any>
#include "abstract_stackable_parser_handler.h"
#include "rapidjson/reader.h"
#include "parser_event.h"
#include "fake_std_throw_length_error.h"

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
        this->add(root_parser_handler);
        _reader = reader;
        _reader->IterativeParseInit();
        _input_stream = input_stream;
    }

    void add(AbstractStackableParserHandler* new_parser_handler)
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
        if (this->_reader->IterativeParseComplete())
        {
            return false;
        }

        this->_reader->IterativeParseNext<rapidjson::kParseDefaultFlags>(
            *(this->_input_stream),
            *(this->_current_parser_handler)
        );

        switch (this->_current_parser_handler->last_parse_event )
        {
        case ParserEvent::IMMERSE_TO_SUBPARSER: {
            BN_LOG("Parsers stack: immerse to subparser");
            AbstractStackableParserHandler* subparser = (AbstractStackableParserHandler*)(this->_current_parser_handler->subparser);
            this->add(subparser);
        //     this->add((AbstractStackableParserHandler*)this->_current_parser_handler->subparser);
            break;
        }
        case ParserEvent::EVENT_PARSE_FINISHED: {
            this->pop();
            this->_current_parser_handler->subparser_finished();
            break;
        }
        // default:
        //     break;
        }

        return true;
    }
};

#endif // PARSERS_STACK_H