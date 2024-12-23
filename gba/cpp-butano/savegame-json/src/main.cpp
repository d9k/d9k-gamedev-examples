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
#include "demo_parse_handler.h"
#include "movie.h"
#include "parsers_stack.h"

BN_DATA_EWRAM char *palestinian_movies_cut_json;

BN_DATA_EWRAM char palestinian_movies_cut_json_begin[255];

using namespace std::string_literals;

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
    delete[] palestinian_movies_cut_json;
}


void parseBigJsonMovies() {
    BN_LOG("\n\n# Parsing big JSON movies\n");

    palestinian_movies_cut_json = (char*)
        #include "palestinian_movies_cut_json.h"
    ;

    TAbstractStackableParserHandler<> root_handler;
    rapidjson::Reader reader;
    rapidjson::StringStream ssBig(palestinian_movies_cut_json);

    ParsersStack *parsersStack = new ParsersStack(&root_handler, &reader, &ssBig);

    while(parsersStack->parse_next_token()) {}

    // reader2.Parse(ssBig, handler2);

    delete parsersStack;
    delete[] palestinian_movies_cut_json;
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


    int *numbers;
    int c = 10;

    numbers = new int[c];

    for (int i = 0; i < c; i++) {
        numbers[i] = i*i;
    }

    BN_LOG("numbers[7]: ", numbers[7]);



    BN_LOG("BN_CFG_LOG_MAX_SIZE: ", BN_CFG_LOG_MAX_SIZE);

    parseSmallJson();
    parseBigJson();
    parseBigJsonMovies();

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
