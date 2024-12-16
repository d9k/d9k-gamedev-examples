/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#define BN_CFG_LOG_MAX_SIZE 1024

#include <sstream>
#include <stdio.h>

#include "bn_core.h"
#include "bn_log.h"
#include "bn_sram.h"
#include "bn_string.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"
#include "bn_sstream.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"
#include "rapidjson/reader.h"
#include "movie.h"

BN_DATA_EWRAM char *palestinian_movies_cut_json;

BN_DATA_EWRAM char palestinian_movies_cut_json_begin[255];

using namespace std::string_literals;

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


namespace
{
    struct sram_data
    {
        bn::array<char, 32> format_tag;
        int reads_count = 0;
    };
}

// char* substr(char* arr, int begin, int len)
// {
//     char* res = new char[len + 1];
//     for (int i = 0; i < len; i++)
//         res[i] = *(arr + begin + i);
//     res[len] = 0;
//     return res;
// }

void parseSmallJson() {
    BN_LOG("\n\n# Parsing small JSON\n");

    const char json[] = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, -3, 4] } ";

    DemoParseHandler handler1;
    rapidjson::Reader reader1;
    rapidjson::StringStream ss1(json);
    reader1.Parse(ss1, handler1);
}

void parseBigJson() {
    BN_LOG("\n\n# Parsing big JSON\n");

    palestinian_movies_cut_json = (char*)
        #include "palestinian_movies_cut_json.h"
    ;

    int FIRST_CHARS = 200;

    std::strncpy(palestinian_movies_cut_json_begin, palestinian_movies_cut_json, FIRST_CHARS);

    DemoParseHandler handler2;
    rapidjson::Reader reader2;
    rapidjson::StringStream ssBig(palestinian_movies_cut_json);
    reader2.Parse(ssBig, handler2);

    BN_LOG("Long JSON first chars (", FIRST_CHARS, "):", palestinian_movies_cut_json_begin);
    palestinian_movies_cut_json = (char *) "";
}

int main()
{
    // concatenation fail with error undefined reference to `std::__throw_length_error(char const*)'
    // std::string testStdString = "std::string"s + " test "s + "string"s + " concat"s;
    // char *testConcat = "test " + "string" + " concat";
    // BN_LOG(testConcat);
    // BN_LOG(testStdString.c_str());

    // BN_DATA_EWRAM int test;
    bn::core::init();

    BN_LOG("BN_CFG_LOG_MAX_SIZE: ", BN_CFG_LOG_MAX_SIZE);

    parseSmallJson();
    parseBigJson();

    // int sram_size = bn::hw::sram::size();
    // BN_LOG("SRAM size:", sram_size);

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    bn::string_view info_text_lines[5];

    sram_data cart_sram_data;
    bn::sram::read(cart_sram_data);

    bn::array<char, 32> expected_format_tag;
    bn::istring_base expected_format_tag_istring(expected_format_tag._data);
    bn::ostringstream expected_format_tag_stream(expected_format_tag_istring);
    expected_format_tag_stream.append("SRAM example");

    bn::string<32> sram_reads_count;

    if(cart_sram_data.format_tag == expected_format_tag)
    {
        ++cart_sram_data.reads_count;

        sram_reads_count = bn::to_string<32>(cart_sram_data.reads_count);

        info_text_lines[0] = "SRAM is formatted!";
        info_text_lines[1] = "";
        info_text_lines[2] = "SRAM reads count:";
        info_text_lines[3] = sram_reads_count;
        info_text_lines[4] = "";
    }
    else
    {
        cart_sram_data.format_tag = expected_format_tag;
        cart_sram_data.reads_count = 1;

        info_text_lines[0] = "Formatting SRAM";
        info_text_lines[1] = "";
        info_text_lines[2] = "If you see this message again,";
        info_text_lines[3] = "SRAM is not working";
        info_text_lines[4] = "Please restart ROM manually";

        bn::sram::clear(bn::sram::size());
    }

    bn::sram::write(cart_sram_data);

    common::info info("SRAM", info_text_lines, text_generator);
    info.set_show_always(true);

    while(true)
    {
        info.update();
        bn::core::update();
    }
}
