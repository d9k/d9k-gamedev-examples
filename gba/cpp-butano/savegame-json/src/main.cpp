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
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "demo_parse_handler.h"
#include "log_long_chars.h"
#include "movie.h"
#include "parsers_stack.h"
#include "savegame_parser.h"
#include "savegame_serializer.h"

using namespace std::string_literals;

namespace
{
    struct sram_data
    {
        bn::array<char, 32> format_tag;
        int reads_count = 0;
    };
}

BN_DATA_EWRAM SaveGame saveGame;

BN_DATA_EWRAM char *palestinian_movies_cut_json;

BN_DATA_EWRAM char palestinian_movies_cut_json_begin[255];

// char* substr(char* arr, int begin, int len)
// {
//     char* res = new char[len + 1];
//     for (int i = 0; i < len; i++)
//         res[i] = *(arr + begin + i);
//     res[len] = 0;
//     return res;
// }

void testStdString()
{
    // concatenation fail with error undefined reference to `std::__throw_length_error(char const*)'
    // std::string testStdString = "std::string"s + " test "s + "string"s + " concat"s;
    std::string testStdString = "std::string";
    // char *testConcat = "test " + "string" + " concat";
    BN_LOG("testStdString: ", testStdString.c_str());

    // TODO: linker error
    // testStdString.assign("another std::string");
    // BN_LOG("testStdString (2): ", testStdString.c_str());
}

void testBnStringView()
{
    bn::string_view *v = new bn::string_view("bn::string_view test");

    BN_LOG("string view test:", v, "size:", v->size());

    delete v;
}

void parseSmallJson()
{
    BN_LOG("\n\n# Parsing small JSON\n");

    const char json[] = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, -3, 4] } ";

    DemoParseHandler handler1;
    rapidjson::Reader reader1;
    rapidjson::StringStream ss1(json);
    reader1.Parse(ss1, handler1);
}

void parseBigJson()
{
    BN_LOG("\n\n# Parsing big JSON\n");

    palestinian_movies_cut_json = (char *)
#include "data_palestinian_movies_cut_json.h"
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

void parseBigJsonMovies()
{
    BN_LOG("\n\n# Parsing big JSON movies\n");

    palestinian_movies_cut_json = (char *)
#include "data_palestinian_movies_cut_json.h"
        ;

    SaveGameParserHandler *root_handler;
    // root_handler = (AbstractStackableParserHandler *)new AbstractStackableParserHandler();
    root_handler = new SaveGameParserHandler();
    rapidjson::Reader reader;
    rapidjson::StringStream ssBig(palestinian_movies_cut_json);

    ParsersStack *parsersStack = new ParsersStack((AbstractStackableParserHandler *)root_handler, &reader, &ssBig);

    while (parsersStack->parse_next_token())
    {
    }

    // reader2.Parse(ssBig, handler2);

    // SaveGame *saveGame = root_handler->get_result();
    saveGame = *root_handler->get_result();

    delete parsersStack;
    delete root_handler;
    delete[] palestinian_movies_cut_json;
}

void debugLogSaveGame() {
    BN_LOG("\n\n# Debug log savegame\n");

    rapidjson::StringBuffer sbuf;
    rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(sbuf);

    serialize_savegame(&jsonWriter, &saveGame);

    // const char* json = chars_copy(sbuf.GetString());

    log_long_chars(sbuf.GetString(), 200);

    // delete json;

    BN_LOG();

    for (int i = 0; i < saveGame.movies.size(); i++)
    {
        Movie *movie = saveGame.movies[i];
        char log_string[256];
        std::sprintf(
            log_string,
            "movies[%2d].plotText: %s",
            i,
            movie->plotText
        );
        BN_LOG(log_string);
        // BN_LOG("movie ", i, ". ", movie->id);
    }

    BN_LOG();

    for (int i = 0; i < saveGame.movies.size(); i++)
    {
        Movie *movie = saveGame.movies[i];
        char log_string[256];
        std::sprintf(
            log_string,
            "(id: %10s) movies[%2d]: (%4d) %s",
            movie->id,
            i,
            movie->year,
            movie->title
        );
        BN_LOG(log_string);
        // BN_LOG("movie ", i, ". ", movie->id);
    }
}

int main()
{
    // BN_LOG(testConcat);
    // BN_LOG(testStdString.c_str());

    // BN_DATA_EWRAM int test;
    bn::core::init();

    testStdString();
    testBnStringView();

    int *numbers;
    int c = 10;

    numbers = new int[c];

    for (int i = 0; i < c; i++)
    {
        numbers[i] = i * i;
    }

    BN_LOG("numbers[7]: ", numbers[7]);

    BN_LOG("BN_CFG_LOG_MAX_SIZE: ", BN_CFG_LOG_MAX_SIZE);

    parseSmallJson();
    parseBigJson();
    parseBigJsonMovies();
    debugLogSaveGame();

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

    if (cart_sram_data.format_tag == expected_format_tag)
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

    while (true)
    {
        info.update();
        bn::core::update();
    }
}
