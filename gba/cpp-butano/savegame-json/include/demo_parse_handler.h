#ifndef DEMO_PARSE_HANDLER_H
#define DEMO_PARSE_HANDLER_H

#include "rapidjson_inc_no_warns.h"

/** [RapidJSON: SAX](https://rapidjson.org/md_doc_sax.html) */
struct DemoParseHandler : public rapidjson::BaseReaderHandler<rapidjson::UTF8<>, DemoParseHandler> {
    uint64_t tokensCount = 0;

    inline bool _logString(const char* str, rapidjson::SizeType length, bool copy, const char * logPrefix = "string") {
        tokensCount++;
        BN_LOG("#", tokensCount, ": ", logPrefix, " \"", str, "\"; len: ", length, copy ? "" : "not copy");
        return true;
    }

    template<typename T>
    inline bool _logToken(const T value, const char * logPrefix = "") {
        tokensCount++;
        BN_LOG("#", tokensCount, ": ", logPrefix, strlen(logPrefix) > 0 ? " " : "", value);
        return true;
    }

    bool String(const char* str, rapidjson::SizeType length, bool copy) {
        return _logString(str, length, copy);
    }

    bool Null() {
        return _logToken("null");
    }

    bool Bool(bool b) {
        // cout << "Bool(" << std::boolalpha << b << ")" << endl; return true;
        return _logToken(b, "bool");
    }

    bool Int(int i) {
        return _logToken(i, "int");
    }

    bool Uint(unsigned u) {
        return _logToken(u, "uint");
    }

    bool Int64(int64_t i) {
        return _logToken(i, "int64");
    }

    bool Uint64(uint64_t u) {
        return _logToken(u, "uint64");
    }

    bool Double(double d) {
        char doubleText[32];
        sprintf(doubleText, "%.6f", d);
        return _logToken(doubleText, "double");
    }

    bool StartObject() {
        return _logToken("start object {");
    }

    bool EndObject(rapidjson::SizeType memberCount) {
        char objectText[32];
        sprintf(objectText, "} end object with %d members", memberCount);
        return _logToken(objectText);
    }

    bool Key(const char* str, rapidjson::SizeType length, bool copy) {
        return _logString(str, length, copy, "key");
    }

    bool StartArray() {
        return _logToken("start array [");
    }

    bool EndArray(rapidjson::SizeType elementCount) {
        char objectText[32];
        sprintf(objectText, "] end array with %d elements", elementCount);
        return _logToken(objectText);
    }
};

#endif // DEMO_PARSE_HANDLER_H