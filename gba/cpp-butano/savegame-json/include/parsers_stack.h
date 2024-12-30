#ifndef PARSERS_STACK_H
#define PARSERS_STACK_H

// #include <list>
// #include <vector>
#include <any>
#include "bn_vector.h"
#include "abstract_stackable_parser_handler.h"
#include "rapidjson_inc_no_warns.h"
#include "parsers_factory.h"

typedef typename rapidjson::UTF8<>::Ch Ch;

/**
 * For iterative reading
 *
 * https://rapidjson.org/md_doc_sax.html
 */
class ParsersStack
{
public:
    // std::vector<AbstractStackableParserHandler *> _parsers_handlers;
    bn::vector<AbstractStackableParserHandler *, 32> _parsers_handlers;
    AbstractStackableParserHandler *_current_parser_handler;
    rapidjson::Reader *_reader;
    rapidjson::StringStream *_input_stream;

    JsonInsideStack shared_json_inside_stack;

    ParsersStack(AbstractStackableParserHandler *root_parser_handler, rapidjson::Reader *reader, rapidjson::StringStream *input_stream)
    {
        // _parsers_handlers = {};
        this->add(root_parser_handler);
        _reader = reader;
        _reader->IterativeParseInit();
        _input_stream = input_stream;
    }

    // void add(AbstractStackableParserHandler *new_parser_handler, bool subparser_inc_level = false)
    void add(AbstractStackableParserHandler *new_parser_handler)
    {
        BN_ASSERT(new_parser_handler != NULL, "ParsersStack: add(): new_parser_handler NULL");

        _parsers_handlers.push_back(new_parser_handler);
        _current_parser_handler = new_parser_handler;
        new_parser_handler->set_json_inside_stack(&this->shared_json_inside_stack);
        // last_parser->last_parse_event;
        // last_parser->parser_name;
        // last_parser->result;
    }

    bool pop()
    {
        if (_parsers_handlers.size() <= 1)
        {
            BN_LOG("ParsersStack: can't pop, size:", _parsers_handlers.size());
            return false;
        }
        // BN_LOG("ParsersStack: pop: 100");
        _parsers_handlers.pop_back();
        // BN_LOG("ParsersStack: pop: 200");
        _current_parser_handler = (AbstractStackableParserHandler *)(_parsers_handlers.back());
        // BN_LOG("ParsersStack: pop: 300");
        // BN_LOG("Popped to ", _current_parser_handler->parser_name());

        return true;
    }

    bool parse_next_token()
    {
        if (this->_reader->IterativeParseComplete())
        {
            BN_LOG("Parsers stack: IterativeParseComplete");
            return false;
        }

        // BN_LOG("ParsersStack: parse next token: 100");

        // BN_LOG("ParsersStack: parse next token: 150: ", this->_current_parser_handler->parser_name());

        // this->_current_parser_handler->last_parse_event = 555;

        // BN_LOG("ParsersStack: parse next token: 170: ", this->_current_parser_handler->last_parse_event);

        // const Ch *input_stream_current_pos = _input_stream->src_;

        this->_reader->IterativeParseNext<rapidjson::kParseDefaultFlags>(
            *(this->_input_stream),
            *(this->_current_parser_handler));

        // BN_LOG("ParsersStack: parse next token: 200");
        // BN_LOG("ParsersStack: parse next token: 250: last parse event:", this->_current_parser_handler->last_parse_event);

        if (this->_current_parser_handler->finished)
        {
            AbstractStackableParserHandler *subparser = this->_current_parser_handler;
            // BN_LOG("ParsersStack: parse_next_token(): finished: 300");
            bool pop_result = this->pop();
            // BN_LOG("ParsersStack: parse_next_token(): finished: 400");
            if (pop_result == false)
            {
                // BN_LOG("ParsersStack: parse_next_token(): finished: 500");
                return false;
            }
            // BN_LOG("ParsersStack: parse_next_token(): finished: 600");
            subparser->destruct_result = this->_current_parser_handler->subparser_finished_returns_if_destruct_result(subparser->parse_result);
            // BN_LOG("ParsersStack: parse_next_token(): finished: 700");
            delete subparser;
            // BN_LOG("ParsersStack: parse_next_token(): finished: 800");
            return true;
        }

        int subparser_id = this->_current_parser_handler->subparser_type_id;

        if (subparser_id)
        {
            AbstractStackableParserHandler *subparser = create_parser_handler_from_type_id(subparser_id);
            // bool subparser_inc_level = this->_current_parser_handler->subparser_inc_level;
            this->add(subparser);
        }

        return true;
    }
};

#endif // PARSERS_STACK_H