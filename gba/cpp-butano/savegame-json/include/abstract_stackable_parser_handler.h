#ifndef ABSTRACT_STACKABLE_PARSER_HANDLER_H
#define ABSTRACT_STACKABLE_PARSER_HANDLER_H

#include <any>
#include <string>
#include "parser_event.h"
#include "bn_string.h"
#include "rapidjson/reader.h"

constexpr int KEY_SIZE = 64;

template <typename T>
struct TAbstractStackableParserHandler : public rapidjson::BaseReaderHandler<rapidjson::UTF8<>>
{
// public:
    TAbstractStackableParserHandler<std::any>* subparser;
    // std::any subparser;
    uint64_t tokens_count = 0;
    int object_level = 0;
    int array_level = 0;
    char current_key[KEY_SIZE];

    T result;
    ParserEvent last_parse_event = ParserEvent::WAIT_NEXT_TOKEN;

    TAbstractStackableParserHandler()
    {
        result_init();
    };

    void result_init()
    {
    }

    virtual char* parser_name() {
        return (char*)"AbstractStackableParser";
    }

    virtual void subparser_finished() {

    }

    bool String(const char *str, rapidjson::SizeType length, bool copy)
    {
        process_token_begin();
        return _logTokenString(str, length, copy);
    }

    bool Null()
    {
        process_token_begin();
        return _logToken("null");
    }

    bool Bool(bool b)
    {
        process_token_begin();
        return _logToken(b, "bool");
    }

    bool Int(int i)
    {
        process_token_begin();
        return _logToken(i, "int");
    }

    bool Uint(unsigned u)
    {
        process_token_begin();
        return _logToken(u, "uint");
    }

    bool Int64(int64_t i)
    {
        process_token_begin();
        return _logToken(i, "int64");
    }

    bool Uint64(uint64_t u)
    {
        process_token_begin();
        return _logToken(u, "uint64");
    }

    bool Double(double d)
    {
        process_token_begin();
        char doubleText[32];
        sprintf(doubleText, "%.6f", d);
        return _logToken(doubleText, "double");
    }

    bool StartObject()
    {
        process_token_begin();
        return _logToken("start object {");
    }

    bool EndObject(rapidjson::SizeType memberCount)
    {
        process_token_begin();
        char objectText[32];
        sprintf(objectText, "} end object with %d members", memberCount);
        return _logToken(objectText);
    }

    bool Key(const char *str, rapidjson::SizeType length, bool copy)
    {
        process_token_begin();
        // current_key = string_from_chars(str);
        // if (current_key) {
            // delete[] current_key;
        // }
        // current_key = chars_from_const_chars(str, length);
        // current_key = (char *)str;

        std::strncpy(current_key, str, length);
        current_key[length] = 0;
        // current_key = bn::string<KEY_SIZE>(str, length);
        bool result = process_key(str, length, copy);
        process_token_end();
        return result;
    }

    bool StartArray()
    {
        return _logToken("start array [");
    }

    bool EndArray(rapidjson::SizeType elementCount)
    {
        char objectText[32];
        sprintf(objectText, "] end array with %d elements", elementCount);
        return _logToken(objectText);
    }

    bool _logTokenString(const char *str, rapidjson::SizeType length, bool copy, const char *logPrefix = "string")
    {
        BN_LOG(this->parser_name(), ": #", tokens_count, ": ", logPrefix, " \"", str, "\"; len: ", length, copy ? "" : "not copy");
        return true;
    }

    template <typename R>
    bool _logToken(const R value, const char *logPrefix = "")
    {
        BN_LOG(this->parser_name(), ": #", tokens_count, ": ", logPrefix, strlen(logPrefix) > 0 ? " " : "", value);
        return true;
    }

    void process_token_begin() {
        this->tokens_count++;
        this->last_parse_event = ParserEvent::WAIT_NEXT_TOKEN;
        if (subparser != NULL) {
            delete subparser;
        }
        subparser = NULL;
    }

    void process_token_end() {
        if (subparser != NULL) {
           this->last_parse_event = ParserEvent::IMMERSE_TO_SUBPARSER;
        }
    }

    virtual bool process_key(const char *str, rapidjson::SizeType length, bool copy) {
        return _logTokenString(str, length, copy, "key");
    }
};

// template <typename T>
// char* TAbstractStackableParserHandler<T>::parser_name()
// {
//     return "AbstractStackableParser";
// }

typedef TAbstractStackableParserHandler<std::any> AbstractStackableParserHandler;

#endif // ABSTRACT_STACKABLE_PARSER_HANDLER_H