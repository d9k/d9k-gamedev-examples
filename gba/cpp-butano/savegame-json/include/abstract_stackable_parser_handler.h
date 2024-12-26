#ifndef ABSTRACT_STACKABLE_PARSER_HANDLER_H
#define ABSTRACT_STACKABLE_PARSER_HANDLER_H

#include <any>
#include <string>
// #include "bn_any.h"
#include "bn_log.h"
#include "bn_string.h"
#include "rapidjson/reader.h"
#include "json_inside_stack.h"

constexpr int KEY_SIZE = 64;

// typedef bn::string<KEY_SIZE> KeyString;

template <typename T>
struct TAbstractStackableParserHandler : public rapidjson::BaseReaderHandler<rapidjson::UTF8<>>
{
    std::any result;
    uint64_t tokens_count = 0;
    int subparser_type_id = 0;
    bool finished = false;
    // KeyString current_key;
    char current_key[KEY_SIZE];
    int start_level = 0;
    bool destruct_result = true;

    JsonInsideStack *shared_json_inside_stack;

    TAbstractStackableParserHandler() {
    };

    ~TAbstractStackableParserHandler() {
    };

    virtual T get_result()
    {
        return std::any_cast<T>(result);
    }

    virtual char *parser_name()
    {
        return (char *)"AbstractStackableParser";
    }

    /** return whether destruct result */
    virtual bool subparser_finished(std::any subparser_result)
    {
        return true;
    }

    bool String(const char *str, rapidjson::SizeType length, bool copy)
    {
        process_token_begin();
        return process_string(str, length, copy);
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
        this->shared_json_inside_stack->add(JsonInsideStack::INSIDE_OBJECT);
        this->log_inside();
        bool result = process_start_object();
        process_token_end();
        return result;
    }

    bool EndObject(rapidjson::SizeType memberCount)
    {
        process_token_begin();
        this->shared_json_inside_stack->pop();
        this->log_inside();
        this->auto_finish_after_close_bracket();
        return process_end_object(memberCount);
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
        // current_key = str;
        bool result = process_key(str, length, copy);
        process_token_end();
        return result;
    }

    bool StartArray()
    {
        this->shared_json_inside_stack->add(JsonInsideStack::INSIDE_ARRAY);
        this->log_inside();
        BN_LOG("## inside: ", this->shared_json_inside_stack->debug_string());
        return this->process_start_array();
    }

    bool EndArray(rapidjson::SizeType elementCount)
    {
        this->shared_json_inside_stack->pop();
        this->log_inside();
        // BN_LOG(this->shared_json_inside_stack->debug_string());
        this->auto_finish_after_close_bracket();
        return this->process_end_array(elementCount);
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

    void process_token_begin()
    {
        this->subparser_type_id = 0;
        this->tokens_count++;
    }

    void process_token_end()
    {
    }

    virtual bool process_start_array()
    {
        return _logToken("start array [");
    }

    virtual bool process_end_array(rapidjson::SizeType elementCount)
    {
        char objectText[32];
        sprintf(objectText, "] end array with %d elements", elementCount);
        return _logToken(objectText);
    }

    virtual bool process_start_object()
    {
        return _logToken("start object {");
    }

    virtual bool process_end_object(rapidjson::SizeType memberCount)
    {
        char objectText[32];
        sprintf(objectText, "} end object with %d members", memberCount);
        return _logToken(objectText);
    }

    virtual bool process_key(const char *str, rapidjson::SizeType length, bool copy)
    {
        return _logTokenString(str, length, copy, "key");
    }

    virtual bool process_string(const char *str, rapidjson::SizeType length, bool copy)
    {
        return _logTokenString(str, length, copy);
    }

    void set_json_inside_stack(JsonInsideStack *json_inside_stack)
    {
        this->shared_json_inside_stack = json_inside_stack;
        this->set_start_level_from_current();
    }

    inline void set_start_level_from_current() {
        this->start_level = this->get_inside_stack_level();
    }

    inline int get_inside_stack_level() {
        return this->shared_json_inside_stack->level();
    }

    inline bool key_is(const char * value) {
        // BN_LOG(this->parser_name(), ": key_is: ", value, " ", current_key, " ", strcmp(value, current_key) == 0);
        return strcmp(value, current_key) == 0;
    }

    void auto_finish_after_close_bracket()
    {
        if (this->get_inside_stack_level() < this->start_level)
        {
            finished = true;
        }
    }

    void log_inside()
    {
        BN_LOG(
            "## inside (start ",
            this->start_level,
            "): ",
            this->shared_json_inside_stack->debug_string()
        );
    }

    inline void error_no_subparser_found() {
        bn::string<64> errorMsg = "No subparser found for type id ";
        errorMsg += this->parser_name();
        errorMsg += ": ";
        errorMsg += bn::to_string<16>(subparser_type_id);
        BN_ASSERT(false, errorMsg);
    }
};

// template <typename T>
// char* TAbstractStackableParserHandler<T>::parser_name()
// {
//     return "AbstractStackableParser";
// }

typedef TAbstractStackableParserHandler<std::any> AbstractStackableParserHandler;

#endif // ABSTRACT_STACKABLE_PARSER_HANDLER_H