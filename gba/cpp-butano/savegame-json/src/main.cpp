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
#include "bn_display.h"

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

    BN_DATA_EWRAM SaveGame saveGame;

    BN_DATA_EWRAM char *palestinian_movies_cut_json;

    BN_DATA_EWRAM char palestinian_movies_cut_json_begin[255];

    constexpr int text_y_inc = 14;
    constexpr bn::fixed text_y_limit_f = (bn::display::height() / 2) - text_y_inc;
    constexpr int text_y_limit = text_y_limit_f.integer();
    constexpr int text_scene_title_y = -text_y_limit;

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::vector<bn::sprite_ptr, 64> text_sprites;

    void _clear_scene()
    {
        text_sprites.clear();
    }

    void _init_scene(const bn::string_view &sceneTitle)
    {
        _clear_scene();
        BN_LOG("\n\n# ", sceneTitle, "\n");

        text_generator.set_center_alignment();
        text_generator.generate(0, text_scene_title_y, sceneTitle, text_sprites);
    }

    void test_std_string()
    {
        // concatenation fail with error undefined reference to `std::__throw_length_error(char const*)'
        // std::string testStdString = "std::string"s + " test "s + "string"s + " concat"s;
        std::string testStdString = "std::string";
        // char *testConcat = "test " + "string" + " concat";
        BN_LOG("testStdString: ", testStdString.c_str());
        bn::vector<bn::sprite_ptr, 64> text_sprites;

        // TODO: linker error
        // testStdString.assign("another std::string");
        // BN_LOG("testStdString (2): ", testStdString.c_str());
    }

    void test_bn_string_view()
    {
        bn::string_view *v = new bn::string_view("bn::string_view test");

        BN_LOG("string view test:", v, "size:", v->size());

        delete v;
    }

    void parse_small_json()
    {
        _init_scene("Parsing small JSON");
        bn::core::update();

        const char json[] = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, -3, 4] } ";

        DemoParseHandler handler1;
        rapidjson::Reader reader1;
        rapidjson::StringStream ss1(json);
        reader1.Parse(ss1, handler1);
    }

    void parse_big_json()
    {
        _init_scene("Parsing big JSON");
        bn::core::update();

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

    void parse_big_json_movies()
    {
        _init_scene("Parsing big JSON movies");
        bn::core::update();

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

    void debug_log_save_game_object()
    {
        _init_scene("Debug log savegame object");
        bn::core::update();

        rapidjson::StringBuffer sbuf;
        rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(sbuf);
        serialize_savegame(&jsonWriter, &saveGame);
        log_long_chars(sbuf.GetString(), 200);

        BN_LOG();

        for (int i = 0; i < saveGame.movies.size(); i++)
        {
            Movie *movie = saveGame.movies[i];
            char log_string[256];
            std::sprintf(
                log_string,
                "movies[%2d].plotText: %s",
                i,
                movie->plotText);
            BN_LOG(log_string);
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
                movie->title);
            BN_LOG(log_string);
        }
    }
}

int main()
{
    // BN_LOG(testConcat);
    // BN_LOG(testStdString.c_str());

    // BN_DATA_EWRAM int test;
    bn::core::init();

    BN_LOG("BN_CFG_LOG_MAX_SIZE: ", BN_CFG_LOG_MAX_SIZE);

    test_std_string();
    test_bn_string_view();

    parse_small_json();
    parse_big_json();
    parse_big_json_movies();
    debug_log_save_game_object();

    // exit;

    _clear_scene();

    // while (true)
    // {
    //     // info.update();
    //     bn::core::update();
    // }

    int sram_size = bn::hw::sram::size();
    BN_LOG("SRAM size:", sram_size);

    // bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
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
