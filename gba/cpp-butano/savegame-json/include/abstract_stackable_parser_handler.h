#ifndef ABSTRACT_STACKABLE_PARSER_HANDLER_H
#define ABSTRACT_STACKABLE_PARSER_HANDLER_H

#include <any>
#include <string>
// #include "bn_any.h"
#include "bn_log.h"
#include "bn_string.h"
#include "json_inside_stack.h"
#include "rapidjson_inc_no_warns.h"

constexpr int KEY_SIZE = 64;

// typedef bn::string<KEY_SIZE> KeyString;

template <typename T>
struct TAbstractStackableParserHandler : public rapidjson::BaseReaderHandler<rapidjson::UTF8<>>
{
    std::any parse_result;
    uint64_t tokens_count = 0;
    int subparser_type_id = 0;
    bool finished = false;
    bn::string<KEY_SIZE> current_key;
    int start_level = 0;
    bool destruct_result = true;

    JsonInsideStack *shared_json_inside_stack;

    TAbstractStackableParserHandler() {
    };

    virtual ~TAbstractStackableParserHandler() {
    };

    virtual T get_result()
    {
        return std::any_cast<T>(parse_result);
    }

    virtual inline const char *parser_name()
    {
        return "AbstractStackableParser";
    }

    /** Shift start level if first token is object begin */
    virtual inline bool is_object()
    {
        return false;
    }

    /** Shift start level if first token is array begin */
    virtual inline bool is_array()
    {
        return false;
    }

    /** return whether destruct parse_result */
    virtual bool subparser_finished_returns_if_destruct_result(std::any /*subparser_result*/)
    {
        return true;
    }

    bool String(const char *str, rapidjson::SizeType length, bool copy)
    {
        process_token_begin();
        process_string(str, length, copy);
        return true;
    }

    bool Null()
    {
        process_token_begin();
        _logToken("null");
        return true;
    }

    bool Bool(bool b)
    {
        process_token_begin();
        _logToken(b, "bool");
        return true;
    }

    bool Int(int i)
    {
        process_token_begin();
        process_int(i);
        return true;
    }

    bool Uint(unsigned u)
    {
        process_token_begin();
        process_int(u);
        return true;
    }

    bool Int64(int64_t i)
    {
        process_token_begin();
        _logToken(i, "int64");
        // TODO process fn
        return true;
    }

    bool Uint64(uint64_t u)
    {
        process_token_begin();
        _logToken(u, "uint64");
        // TODO process fn
        return true;
    }

    bool Double(double d)
    {
        process_token_begin();
        // char doubleText[32];
        // %f built with DevKitARM corrupts memory (Allocation failed. Size in bytes: ...)
        // sprintf(doubleText, "%.6f", d);
        // TODO process fn
        bn::fixed_t<20> fixed = d;
        bn::string<32> fixed_string = string_from_fixed<32>(fixed);

        _logToken(fixed_string.c_str(), "double");
        return true;
    }

    bool StartObject()
    {
        process_token_begin();
        this->shared_json_inside_stack->add(JsonInsideStack::INSIDE_OBJECT);
        log_json_inside_stack();

        if (this->is_object() && tokens_count == 1)
        {
            set_start_level_from_current();
            _logToken("start object {, update start level");
        }

        process_start_object();

        return true;
    }

    bool EndObject(rapidjson::SizeType memberCount)
    {
        process_token_begin();
        this->shared_json_inside_stack->pop();
        log_json_inside_stack();
        auto_finish_after_close_bracket();
        process_end_object(memberCount);
        return true;
    }

    bool Key(const char *str, rapidjson::SizeType length, bool copy)
    {
        process_token_begin();
        current_key = str;
        // std::strncpy(current_key, str, length);
        // current_key[length] = 0;
        process_key(str, length, copy);
        return true;
    }

    bool StartArray()
    {
        process_token_begin();
        this->shared_json_inside_stack->add(JsonInsideStack::INSIDE_ARRAY);
        log_json_inside_stack();
        BN_LOG("## inside: ", this->shared_json_inside_stack->debug_string());

        if (this->is_array() && tokens_count == 1)
        {
            set_start_level_from_current();
            _logToken("start array [, update start level");
        }

        process_start_array();
        return true;
    }

    bool EndArray(rapidjson::SizeType elementCount)
    {
        process_token_begin();
        this->shared_json_inside_stack->pop();
        log_json_inside_stack();
        auto_finish_after_close_bracket();
        process_end_array(elementCount);
        return true;
    }

    void _logTokenString(const char *str, rapidjson::SizeType length, bool copy, const char *logPrefix = "string")
    {
        BN_LOG(this->parser_name(), ": #", tokens_count, ": ", logPrefix, " \"", str, "\"; len: ", length, copy ? "" : "not copy");
    }

    template <typename R>
    void _logToken(const R value, const char *logPrefix = "")
    {
        BN_LOG(this->parser_name(), ": #", tokens_count, ": ", logPrefix, strlen(logPrefix) > 0 ? " " : "", value);
    }

    void process_token_begin()
    {
        this->subparser_type_id = 0;
        this->tokens_count++;
    }

    virtual void process_int(int i)
    {
        _logToken(i, "int");
    }

    virtual void process_start_array()
    {

        _logToken("start array [");
    }

    virtual void process_end_array(rapidjson::SizeType elementCount)
    {
        char objectText[32];
        sprintf(objectText, "] end array with %d elements", elementCount);
        _logToken(objectText);
    }

    virtual void process_start_object()
    {

        _logToken("start object {");
    }

    virtual void process_end_object(rapidjson::SizeType memberCount)
    {
        char objectText[32];
        sprintf(objectText, "} end object with %d members", memberCount);
        return _logToken(objectText);
    }

    virtual void process_key(const char *str, rapidjson::SizeType length, bool copy)
    {
        _logTokenString(str, length, copy, "key");
    }

    virtual void process_string(const char *str, rapidjson::SizeType length, bool copy)
    {
        _logTokenString(str, length, copy);
    }

    void set_json_inside_stack(JsonInsideStack *json_inside_stack)
    {
        this->shared_json_inside_stack = json_inside_stack;
        set_start_level_from_current();
    }

    inline void set_start_level_from_current()
    {
        start_level = this->get_inside_json_stack_level();
    }

    inline int get_inside_json_stack_level()
    {
        return this->shared_json_inside_stack->level();
    }

    inline bool key_is(const char *value)
    {
        // BN_LOG(this->parser_name(), ": key_is: ", value, " ", current_key, " ", strcmp(value, current_key) == 0);
        // return strcmp(value, current_key) == 0;
        return current_key == value;
    }

    void auto_finish_after_close_bracket()
    {
        if (this->get_inside_json_stack_level() < this->start_level)
        {
            finished = true;
        }
    }

    void log_json_inside_stack()
    {
        BN_LOG(
            "## inside (start ",
            this->start_level,
            "): ",
            this->shared_json_inside_stack->debug_string());
    }

    inline void error_no_subparser_found()
    {
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