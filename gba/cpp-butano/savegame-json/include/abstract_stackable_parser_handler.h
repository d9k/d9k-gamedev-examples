#ifndef ABSTRACT_STACKABLE_PARSER_HANDLER_H
#define ABSTRACT_STACKABLE_PARSER_HANDLER_H

#include <any>
#include "parser_event.h"
#include "rapidjson/reader.h"

template <typename T>
struct TAbstractStackableParserHandler : public rapidjson::BaseReaderHandler<rapidjson::UTF8<>>
{
public:
    uint64_t tokens_count = 0;
    int object_level = 0;
    int array_level = 0;

    // char *parser_name = "AbstractStackableParser";
    T result;
    ParserEvent last_parse_event = ParserEvent::WAIT_NEXT_TOKEN;

    TAbstractStackableParserHandler()
    {
        result_init();
        //     // parser_handler
    };

    void result_init()
    {
    }

    virtual char* parser_name();

    bool String(const char *str, rapidjson::SizeType length, bool copy)
    {
        return _logTokenString(str, length, copy);
    }

    bool Null()
    {
        return _logToken("null");
    }

    bool Bool(bool b)
    {
        // cout << "Bool(" << std::boolalpha << b << ")" << endl; return true;
        return _logToken(b, "bool");
    }

    bool Int(int i)
    {
        return _logToken(i, "int");
    }

    bool Uint(unsigned u)
    {
        return _logToken(u, "uint");
    }

    bool Int64(int64_t i)
    {
        return _logToken(i, "int64");
    }

    bool Uint64(uint64_t u)
    {
        return _logToken(u, "uint64");
    }

    bool Double(double d)
    {
        char doubleText[32];
        sprintf(doubleText, "%.6f", d);
        return _logToken(doubleText, "double");
    }

    bool StartObject()
    {
        return _logToken("start object {");
    }

    bool EndObject(rapidjson::SizeType memberCount)
    {
        char objectText[32];
        sprintf(objectText, "} end object with %d members", memberCount);
        return _logToken(objectText);
    }

    bool Key(const char *str, rapidjson::SizeType length, bool copy)
    {
        return _logTokenString(str, length, copy, "key");
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
        tokens_count++;
        BN_LOG(this->parser_name(), ": #", tokens_count, ": ", logPrefix, " \"", str, "\"; len: ", length, copy ? "" : "not copy");
        return true;
    }

    template <typename R>
    bool _logToken(const R value, const char *logPrefix = "")
    {
        tokens_count++;
        BN_LOG(this->parser_name(), ": #", tokens_count, ": ", logPrefix, strlen(logPrefix) > 0 ? " " : "", value);
        return true;
    }
};

template <typename T>
char* TAbstractStackableParserHandler<T>::parser_name()
{
    return "AbstractStackableParser";
}

typedef TAbstractStackableParserHandler<std::any> AbstractStackableParserHandler;

#endif // ABSTRACT_STACKABLE_PARSER_HANDLER_H