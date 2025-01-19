/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#define BN_CFG_LOG_MAX_SIZE 1024

#include <sstream>
#include <stdio.h>

#include "bn_bg_palettes.h"
#include "bn_core.h"
#include "bn_display.h"
#include "bn_keypad.h"
#include "bn_log.h"
#include "bn_sprite_text_generator.h"
#include "bn_sstream.h"
#include "bn_string.h"

#include "const.h"
#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"
#include "rapidjson_inc_no_warns.h"
#include "demo_parse_handler.h"
#include "log_long_chars.h"
#include "movie.h"
#include "sram.h"
#include "parsers_stack.h"
#include "savegame_parser.h"
#include "savegame_serializer.h"
#include "semver_from_chars.h"

#include "screen_text/rows_composer.h"
#include "screen_text/title.h"
#include "screen_text/caption_value_pair.h"
#include "screen_text/scrollable_block.h"

using namespace std::string_literals;

namespace
{
    // struct sram_data
    // {
    //     bn::array<char, 32> format_tag;
    //     int reads_count = 0;
    // };

    BN_DATA_EWRAM SaveGame *save_game;

    BN_DATA_EWRAM const char *palestinian_movies_cut_json =
#include "data_palestinian_movies_cut_json.h"
        ;

    constexpr int text_y_inc = 14;
    constexpr bn::fixed text_y_limit_f = (bn::display::height() / 2) - text_y_inc;
    constexpr int text_y_limit = text_y_limit_f.integer();
    constexpr int text_scene_title_y = -text_y_limit;
    constexpr int rows_composer_first_row_cy_shift = -bn::display::height() / 2 + 16 / 2;
    constexpr int rows_composer_line_height = 18;
    constexpr int key_value_pair_cx_shift_default = 0;

    /** Needed to properly fill savegame differences with zeroes */
    int sram_old_usage = 0;

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::vector<bn::sprite_ptr, 64> text_sprites;

    void _clear_scene()
    {
        text_sprites.clear();
    }

    void _set_save_game(SaveGame *new_save_game)
    {
        delete save_game;
        save_game = new_save_game;
    }

    void _init_scene(const bn::string_view &sceneTitle)
    {
        _clear_scene();
        BN_LOG("\n\n# ", sceneTitle, "\n");

        text_generator.set_center_alignment();
        text_generator.generate(0, text_scene_title_y, sceneTitle, text_sprites);
    }

    void _sram_write_save_game()
    {
        Sram sram;
        sram.save(save_game, sram_old_usage);
    }

    void test_semver_by_neargye()
    {
        BN_LOG("\n\n# Test semver by Neargye\n");
        const char *version_string = "1.2.3";

        semver::version v = semver_from_chars(version_string);
        semver::version v_older = semver_from_chars("1.2.0");
        semver::version v_newer = semver_from_chars("1.3.3");

        BN_LOG("test_semver_by_neargye(): v: ", v.major, " ", v.minor, " ", v.patch);
        BN_LOG("test_semver_by_neargye(): v_older: ", v_older.major, " ", v_older.minor, " ", v_older.patch);
        BN_LOG("test_semver_by_neargye(): v_newer: ", v_newer.major, " ", v_newer.minor, " ", v_newer.patch);

        bool v_less_then_v_older = v < v_older;

        BN_LOG("test_semver_by_neargye(): v < v_older: ", v_less_then_v_older);
        BN_LOG("test_semver_by_neargye(): v > v_older: ", v > v_older);

        BN_LOG("test_semver_by_neargye(): v < v_newer: ", v < v_newer);
        BN_LOG("test_semver_by_neargye(): v > v_newer: ", v > v_newer);
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

        int FIRST_CHARS = 200;

        char palestinian_movies_cut_json_begin[255];

        std::strncpy(palestinian_movies_cut_json_begin, palestinian_movies_cut_json, FIRST_CHARS);

        BN_LOG("Long JSON first chars (", FIRST_CHARS, "):", palestinian_movies_cut_json_begin);

        DemoParseHandler handler2;
        rapidjson::Reader reader2;
        rapidjson::StringStream ssBig(palestinian_movies_cut_json);
        reader2.Parse(ssBig, handler2);
    }

    void parse_big_json_movies()
    {
        _init_scene("Parsing big JSON movies");
        bn::core::update();

        SaveGameParserHandler *root_handler;
        root_handler = new SaveGameParserHandler();
        rapidjson::Reader reader;
        rapidjson::StringStream ssBig(palestinian_movies_cut_json);

        ParsersStack *parsersStack = new ParsersStack((AbstractStackableParserHandler *)root_handler, &reader, &ssBig);

        while (parsersStack->parse_next_token())
        {
        }

        _set_save_game(root_handler->get_result());
        root_handler->destruct_result = false;

        delete parsersStack;
        delete root_handler;
    }

    void debug_log_save_game_object()
    {
        _init_scene("Debug log savegame object");
        bn::core::update();

        rapidjson::StringBuffer sbuf;
        rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(sbuf);
        serialize_savegame(&jsonWriter, save_game);
        log_long_chars(sbuf.GetString(), 200, "debug log save game json");

        BN_LOG("\n");

        for (uint32_t i = 0; i < save_game->movies.size(); i++)
        {
            Movie *movie = save_game->movies[i];
            char log_string[256];
            std::sprintf(
                log_string,
                "movies[%2lu].plotText: %s",
                i,
                movie->plot_text.get_chars());
            BN_LOG(log_string);
        }

        BN_LOG("\n");

        for (uint32_t i = 0; i < save_game->movies.size(); i++)
        {
            Movie *movie = save_game->movies[i];
            char log_string[256];
            std::sprintf(
                log_string,
                "(id: %10s) movies[%2lu]: (%4d) %s",
                movie->id.get_chars(),
                i,
                movie->year,
                movie->title.get_chars());
            BN_LOG(log_string);
        }
    }

    void reading_sram_scene()
    {
        _init_scene("Reading SRAM");

        // _clear_scene();
        bn::core::update();

        Sram sram;

        BN_LOG("actual SRAM size: ", sram.actual_sram_size);

        bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

        sram::LoadResult sram_load_result = sram.load();

        BN_LOG("Old savegame version in SRAM: ", sram_load_result.old_save_game_version.get_chars());
        BN_LOG("Old SRAM usage:", sram_load_result.sram_old_usage);

        if (sram_load_result.error)
        {
            BN_LOG("There was error during loading savegame from SRAM");
            BN_LOG("Formatting SRAM");
            sram.format();
        }
        else
        {
            _set_save_game(sram_load_result.save_game);
        }

        sram_old_usage = sram_load_result.sram_old_usage;
    }

    void movies_info_viewer_scene()
    {
        _clear_scene();

        screen_text::RowsComposer<64, 64> rows_composer(&text_generator, rows_composer_line_height);
        rows_composer.first_row_cy_shift = rows_composer_first_row_cy_shift;
        rows_composer.key_value_pair_cx_shift_default = key_value_pair_cx_shift_default;

        screen_text::Title title("Palestinian movies info viewer");
        screen_text::CaptionValuePair loads_counter("Loads count");
        screen_text::CaptionValuePair movie_id_display("Movie id");
        screen_text::CaptionValuePair movie_year_display("Year");
        screen_text::Title movie_title_display("", screen_text::ALIGN_CENTER, true);

        rows_composer.add_block(&title);
        rows_composer.add_block(&movie_id_display);
        rows_composer.add_block(&movie_title_display);
        rows_composer.add_block(&movie_year_display);
        rows_composer.add_block(&loads_counter, 6);

        _sram_write_save_game();

        while (!bn::keypad::start_pressed())
        {
            Movie *movie = save_game->get_selected_movie();

            loads_counter.dynamic_value.set_chars(bn::to_string<16>(save_game->loads_count).c_str());

            movie_id_display.dynamic_value.set_chars(save_game->selected_movie_id.get_chars());
            movie_title_display.chars = movie->title.get_chars();
            // BN_LOG("movie title: ", movie->title.get_chars());
            movie_year_display.dynamic_value.set_chars(bn::to_string<16>(movie->year).c_str());

            if (bn::keypad::left_pressed() || bn::keypad::up_pressed())
            {
                save_game->inc_selected_movie_id(-1);
            }

            if (bn::keypad::right_pressed() || bn::keypad::down_pressed())
            {
                save_game->inc_selected_movie_id(1);
            }

            bn::core::update();
            rows_composer.rerender();
        }
    }
}

int main()
{
    bn::core::init();

    BN_LOG("BN_CFG_LOG_MAX_SIZE: ", BN_CFG_LOG_MAX_SIZE);

    test_semver_by_neargye();
    parse_small_json();
    parse_big_json();
    parse_big_json_movies();
    debug_log_save_game_object();
    reading_sram_scene();
    movies_info_viewer_scene();

    //     info_text_lines[0] = "SRAM is formatted!";
    //     info_text_lines[1] = "";
    //     info_text_lines[2] = "SRAM reads count:";
    //     info_text_lines[3] = sram_reads_count;
    //     info_text_lines[4] = "";

    //     info_text_lines[0] = "Formatting SRAM";
    //     info_text_lines[1] = "";
    //     info_text_lines[2] = "If you see this message again,";
    //     info_text_lines[3] = "SRAM is not working";
    //     info_text_lines[4] = "Please restart ROM manually";

    while (true)
    {
        // info.update();
        bn::core::update();
    }
}
