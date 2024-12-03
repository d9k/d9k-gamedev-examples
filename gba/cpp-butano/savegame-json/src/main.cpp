/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_log.h"
#include "bn_sram.h"
// #include "../../../butano/hw/include/bn_hw_sram.h"
#include "bn_string.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"
#include "bn_string.h"
#include "bn_sstream.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"
#include "rapidjson/reader.h"
// #include <iostream>
#include <sstream>

// using std::cout;
// using std::endl;
// #include <cstring>
// #include "string.h"
// #include <stdio.h>
// #include <string.h>
// #include <cstring>

// BN_DATA_EWRAM const char *palestinian_movies_cut_json =
BN_DATA_EWRAM char palestinian_movies_cut_json[32000] =
// BN_DATA_EWRAM char *palestinian_movies_cut_json =
// BN_DATA_EWRAM const std::string palestinian_movies_cut_json =
// BN_DATA_EWRAM bn::string<32000> palestinian_movies_cut_json =
// BN_DATA_EWRAM bn::string<32000> palestinian_movies_cut_json =
    #include "palestinian_movies_cut_json.h"
;

BN_DATA_EWRAM char palestinian_movies_cut_json_begin[255];
// BN_DATA_EWRAM bn::string<255> palestinian_movies_cut_json_begin;
// bn::string<255> palestinian_movies_cut_json_begin;
// BN_DATA_EWRAM std::string palestinian_movies_cut_json_begin;

/** [RapidJSON: SAX](https://rapidjson.org/md_doc_sax.html) */
struct MyHandler : public rapidjson::BaseReaderHandler<rapidjson::UTF8<>, MyHandler> {
    // bool Null() { cout << "Null()" << endl; return true; }
    // bool Bool(bool b) { cout << "Bool(" << std::boolalpha << b << ")" << endl; return true; }
    // bool Int(int i) { cout << "Int(" << i << ")" << endl; return true; }
    // bool Uint(unsigned u) { cout << "Uint(" << u << ")" << endl; return true; }
    // bool Int64(int64_t i) { cout << "Int64(" << i << ")" << endl; return true; }
    // bool Uint64(uint64_t u) { cout << "Uint64(" << u << ")" << endl; return true; }
    // bool Double(double d) { cout << "Double(" << d << ")" << endl; return true; }
    bool String(const char* str, rapidjson::SizeType length, bool copy) {
        bn::string<1024> text;
        bn::ostringstream ss(text);
            ss << "String(" << str << ", " << length << ", " << copy << ")"; // << endl;
        BN_LOG(text);
        return true;
    }
    // bool StartObject() { cout << "StartObject()" << endl; return true; }
    // bool Key(const char* str, rapidjson::SizeType length, bool copy) {
    //     cout << "Key(" << str << ", " << length << ", " << std::boolalpha << copy << ")" << endl;
    //     return true;
    // }
    // bool EndObject(rapidjson::SizeType memberCount) { cout << "EndObject(" << memberCount << ")" << endl; return true; }
    // bool StartArray() { cout << "StartArray()" << endl; return true; }
    // bool EndArray(rapidjson::SizeType elementCount) { cout << "EndArray(" << elementCount << ")" << endl; return true; }
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

int main()
{
    bn::core::init();

    const char json[] = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ";

    MyHandler handler;
    rapidjson::Reader reader;
    rapidjson::StringStream ss(json);
    reader.Parse(ss, handler);

    // int sram_size = bn::hw::sram::size();

    // int wtf = 12345;

    // std::strncpy(palestinian_movies_cut_json_begin, palestinian_movies_cut_json, 200);
    // strncpy(palestinian_movies_cut_json_begin, palestinian_movies_cut_json, 200);
    // strcpy(palestinian_movies_cut_json_begin, palestinian_movies_cut_json, 200);
    // BN_LOG('palestinian_movies_cut_json_begin:',  palestinian_movies_cut_json_begin);
    // BN_LOG('SRAM size:',  bn::hw::sram::size);
    // BN_LOG('SRAM size:', wtf);
    // BN_LOG("SRAM size:", sram_size);

    // palestinian_movies_cut_json_begin = palestinian_movies_cut_json.substr(0, 200);
    // palestinian_movies_cut_json_begin = substr(palestinian_movies_cut_json, 0, 200);

    int len = 200;

    for (int i = 0; i < len; i++) {
        // palestinian_movies_cut_json_begin[i] = *(arr + begin + i);
        palestinian_movies_cut_json_begin[i] = palestinian_movies_cut_json[i];
    }
    palestinian_movies_cut_json_begin[len] = 0;

    rapidjson::StringStream ssBig(palestinian_movies_cut_json);
    reader.Parse(ssBig, handler);

    // BN_LOG('chars: ', palestinian_movies_cut_json[6], palestinian_movies_cut_json[7]);
    BN_LOG("chars: ", palestinian_movies_cut_json_begin);
    // BN_LOG("chars: ", palestinian_movies_cut_json);

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
