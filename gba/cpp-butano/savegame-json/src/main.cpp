#define BN_CFG_SPRITE_TILES_LOG_ENABLED true
/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

// #define BN_CFG_LOG_MAX_SIZE 1024

#include <sstream>
#include <stdio.h>

#include "bn_config_sprite_tiles.h"
#include "bn_bg_palettes.h"
#include "bn_core.h"
#include "bn_display.h"
#include "bn_keypad.h"
#include "bn_log.h"
#include "bn_sprite_text_generator.h"
#include "bn_sstream.h"
#include "bn_string.h"

#include "chars_pointer_copy_wrapper.h"
#include "const.h"
#include "common_info.h"
#include "common_fixed_8x16_sprite_font.h"
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

#include "bn_sprite_items_arrow_down.h"
#include "bn_sprite_items_arrow_left.h"
#include "bn_sprite_items_arrow_right.h"
#include "bn_sprite_items_arrow_up.h"

using namespace std::string_literals;

namespace
{
    namespace scene_id
    {
        constexpr int READING_SRAM = 100;
        constexpr int MOVIES_INFO_VIEWER = 200;
        constexpr int HELP_MOVIES_INFO_VIEWER = 300;
    }

    int scene_id_next = scene_id::READING_SRAM;
    // struct sram_data
    // {
    //     bn::array<char, 32> format_tag;
    //     int reads_count = 0;
    // };

    BN_DATA_EWRAM SaveGame *save_game;

    BN_DATA_EWRAM const char *palestinian_movies_json =
#include "data_palestinian_movies_json.h"
        ;

    constexpr int cx_position_at_right_border = bn::display::width() / 2 - 8;
    constexpr int cx_position_at_left_border = -bn::display::width() / 2 + 8;
    constexpr int text_y_inc = 14;
    constexpr bn::fixed text_y_limit_f = (bn::display::height() / 2) - text_y_inc;
    constexpr int text_y_limit = text_y_limit_f.integer();
    constexpr int text_scene_title_y = -text_y_limit;
    constexpr int rows_composer_first_row_cy_shift = -bn::display::height() / 2 + 16 / 2;
    constexpr int rows_composer_row_height = 18;
    constexpr int key_value_pair_cx_shift_default = 8;
    // constexpr int help_scene_key_value_pair_cx_shift_default = -40;
    constexpr int help_scene_key_value_pair_cx_shift_default = 5;
    // constexpr int scrollable_block_window_rows = 6;
    constexpr int scrollable_block_window_columns = 26;
    constexpr int scrollable_block_row_height = 12;

    /** Needed to properly fill savegame differences with zeroes */
    int sram_old_usage = 0;
    CharsPointerCopyWrapper sram_old_char_wrapper_selected_movie_id;

    bn::sprite_text_generator text_generator(common::fixed_8x16_sprite_font);
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
        sram::SaveResult save_result = sram.save(save_game, sram_old_usage);
        sram_old_usage = save_result.sram_new_usage;
        sram_old_char_wrapper_selected_movie_id.copy_fields_from(&save_game->chars_wrapper_selected_movie_id);
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

        std::strncpy(palestinian_movies_cut_json_begin, palestinian_movies_json, FIRST_CHARS);

        BN_LOG("Long JSON first chars (", FIRST_CHARS, "):", palestinian_movies_cut_json_begin);

        DemoParseHandler handler2;
        rapidjson::Reader reader2;
        rapidjson::StringStream ssBig(palestinian_movies_json);
        reader2.Parse(ssBig, handler2);
    }

    void parse_big_json_movies()
    {
        _init_scene("Parsing big JSON movies");
        bn::core::update();

        SaveGameParserHandler *root_handler;
        root_handler = new SaveGameParserHandler();
        rapidjson::Reader reader;
        rapidjson::StringStream ssBig(palestinian_movies_json);

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
                movie->chars_wrapper_id.get_chars(),
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

        sram_old_usage = sram_load_result.sram_old_usage;

        BN_LOG("Old savegame version in SRAM: ", sram_load_result.old_save_game_version.get_chars());
        BN_LOG("Old SRAM usage:", sram_old_usage);

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

        sram_old_char_wrapper_selected_movie_id.copy_fields_from(&save_game->chars_wrapper_selected_movie_id);

        scene_id_next = scene_id::MOVIES_INFO_VIEWER;
    }

    void movies_info_viewer_scene()
    {
        _clear_scene();
        BN_LOG("\n\n# movies_info_viewer_scene()\n");

        bn::sprite_ptr arrow_down = bn::sprite_items::arrow_down.create_sprite(
            cx_position_at_right_border - 4,
            0);
        arrow_down.set_visible(false);

        bn::sprite_ptr arrow_up = bn::sprite_items::arrow_up.create_sprite(
            cx_position_at_right_border - 4,
            0);
        arrow_up.set_visible(false);

        bn::sprite_ptr arrow_left = bn::sprite_items::arrow_left.create_sprite(
            cx_position_at_left_border,
            0);
        arrow_down.set_visible(false);

        bn::sprite_ptr arrow_right = bn::sprite_items::arrow_right.create_sprite(
            cx_position_at_right_border,
            0);
        arrow_down.set_visible(false);

        screen_text::RowsComposer<384, 128> rows_composer(&text_generator, rows_composer_row_height);
        rows_composer.first_row_cy_shift = rows_composer_first_row_cy_shift;
        rows_composer.key_value_pair_cx_shift_default = key_value_pair_cx_shift_default;

        // screen_text::Title title("Palestinian movies info viewer");
        // screen_text::CaptionValuePair loads_counter("Loads count");
        screen_text::CaptionValuePair display_movie_id("Movie id");

        screen_text::ScrollableBlock scrollable_movie_title("", 2, scrollable_block_window_columns);
        // scrollable_movie_title.row_cx_shift = cx_position_at_left_border - 2;
        scrollable_movie_title.alignment = screen_text::ALIGN_CENTER;
        scrollable_movie_title.custom_row_height = scrollable_block_row_height;

        screen_text::CaptionValuePair display_movie_year("Year");
        display_movie_year.custom_margin_with_last_block = 8;

        // screen_text::Title display_movie_title("");

        screen_text::ScrollableBlock scrollable_plot_text("", 5, scrollable_block_window_columns);
        scrollable_plot_text.row_cx_shift = cx_position_at_left_border - 2;
        scrollable_plot_text.scroll_vertical_delta = 4;
        // scrollable_movie_title.alignment = screen_text::ALIGN_CENTER;
        scrollable_plot_text.custom_row_height = scrollable_block_row_height;

        screen_text::Title title_hotkey_help("(Select: show help)");
        title_hotkey_help.custom_margin_with_last_block = 12;

        // rows_composer.add_block(&title);
        rows_composer.add_block(&display_movie_id);
        rows_composer.add_block(&scrollable_movie_title);
        rows_composer.add_block(&display_movie_year);
        // rows_composer.add_block(&loads_counter, 7);
        rows_composer.add_block(&scrollable_plot_text);
        rows_composer.add_block(&title_hotkey_help, 9);

        int ui_redraws_count = 0;
        bool ui_redraw_required = true;
        bool ui_partial_redraw_required = false;

        do
        {
            ui_redraws_count++;
            bn::core::update();

            if (bn::keypad::left_pressed())
            {
                save_game->inc_selected_movie_id(-1);
                ui_redraw_required = true;
            }

            if (bn::keypad::right_pressed())
            {
                save_game->inc_selected_movie_id(1);
                ui_redraw_required = true;
            }

            if (bn::keypad::up_pressed())
            {
                scrollable_plot_text.dec_scroll_vertical_current();
                ui_partial_redraw_required = true;
            }

            if (bn::keypad::down_pressed())
            {
                scrollable_plot_text.inc_scroll_vertical_current();
                ui_partial_redraw_required = true;
            }

            if (bn::keypad::select_pressed())
            {
                scene_id_next = scene_id::HELP_MOVIES_INFO_VIEWER;
            }

            if (bn::keypad::l_pressed() || bn::keypad::r_pressed())
            {
                scene_id_next = scene_id::READING_SRAM;
            }

            if (bn::keypad::a_pressed() || bn::keypad::b_pressed())
            {
                _sram_write_save_game();
                ui_redraw_required = true;
            }

            Movie *movie = save_game->get_selected_movie();

            // loads_counter.dynamic_value.set_chars(bn::to_string<16>(save_game->loads_count).c_str());

            if (ui_redraw_required)
            {
                ui_redraw_required = false;

                rows_composer.reset();

                const char *chars_selected_movie_id = save_game->chars_wrapper_selected_movie_id.get_chars();
                bn::string<64> string_display_movie_id(chars_selected_movie_id);
                bn::ostringstream string_stream_display_movie_id(string_display_movie_id);

                if (strcmp(chars_selected_movie_id, sram_old_char_wrapper_selected_movie_id.get_chars()) != 0)
                {
                    string_stream_display_movie_id << " *";
                }

                display_movie_id.dynamic_value.set_chars(string_display_movie_id);
                // display_movie_title.chars = movie->title.get_chars();
                scrollable_movie_title.set_static_text(movie->title.get_chars());
                display_movie_year.dynamic_value.set_chars(bn::to_string<16>(movie->year));
                scrollable_plot_text.set_static_text(movie->plot_text.get_chars());
                ui_partial_redraw_required = true;
            }
            if (ui_redraws_count == 2)
            {
                arrow_left.set_y(display_movie_id.rendered_block_cy_shift + 1);
                arrow_right.set_y(display_movie_id.rendered_block_cy_shift + 1);
                arrow_up.set_y(scrollable_plot_text.rendered_block_cy_shift + 4);
                arrow_down.set_y(scrollable_plot_text.rendered_block_cy_shift + scrollable_plot_text.rendered_block_height - 14);
                ui_partial_redraw_required = true;
            }
            if (ui_partial_redraw_required)
            {
                ui_partial_redraw_required = false;
                rows_composer.reset();
                arrow_up.set_visible(scrollable_plot_text.can_scroll_up());
                arrow_down.set_visible(scrollable_plot_text.can_scroll_down());
                arrow_left.set_visible(true);
                arrow_right.set_visible(true);
            }

            rows_composer.rerender();
        } while (scene_id_next == scene_id::MOVIES_INFO_VIEWER);
    }

    void help_movies_info_viewer_scene()
    {
        _clear_scene();
        BN_LOG("\n\n# help_movies_info_viewer_scene()\n");

        screen_text::RowsComposer<128, 64> rows_composer(&text_generator, rows_composer_row_height);

        screen_text::Title title("Palestinian movies info viewer");

        screen_text::CaptionValuePair counter_loads("Loads count");
        counter_loads.custom_row_cx_shift = 45;

        screen_text::Title title_hotkey_return("(Select: return)");

        screen_text::Title title_hotkeys_delimiter("");

        screen_text::CaptionValuePair title_hotkey_switch_movie("Switch movie");
        title_hotkey_switch_movie.dynamic_value.set_chars("Left/Right");

        screen_text::CaptionValuePair title_hotkey_save("Save");
        title_hotkey_save.dynamic_value.set_chars("A/B");

        screen_text::CaptionValuePair title_hotkey_load("Load");
        title_hotkey_load.dynamic_value.set_chars("LB/RB");

        rows_composer.first_row_cy_shift = rows_composer_first_row_cy_shift;
        rows_composer.key_value_pair_cx_shift_default = help_scene_key_value_pair_cx_shift_default;

        rows_composer.add_block(&title);
        rows_composer.add_block(&title_hotkeys_delimiter);
        rows_composer.add_block(&title_hotkey_switch_movie);
        rows_composer.add_block(&title_hotkey_save);
        rows_composer.add_block(&title_hotkey_load);

        rows_composer.add_block(&counter_loads, 7);
        rows_composer.add_block(&title_hotkey_return, 8);

        counter_loads.dynamic_value.set_chars(bn::to_string<16>(save_game->loads_count).c_str());

        do
        {
            bn::core::update();

            rows_composer.rerender();
        } while (!bn::keypad::select_pressed());

        scene_id_next = scene_id::MOVIES_INFO_VIEWER;
    }
}

int main()
{
    bn::core::init();

    BN_LOG("BN_CFG_LOG_MAX_SIZE: ", BN_CFG_LOG_MAX_SIZE);
    BN_LOG("BN_CFG_SPRITE_TILES_MAX_ITEMS: ", BN_CFG_SPRITE_TILES_MAX_ITEMS);

    test_semver_by_neargye();
    parse_small_json();
    parse_big_json();
    parse_big_json_movies();
    debug_log_save_game_object();

    while (true)
    {
        switch (scene_id_next)
        {
        case scene_id::READING_SRAM:
        {
            reading_sram_scene();
            break;
        }
        case scene_id::MOVIES_INFO_VIEWER:
        {
            movies_info_viewer_scene();
            break;
        }
        case scene_id::HELP_MOVIES_INFO_VIEWER:
        {
            help_movies_info_viewer_scene();
            break;
        }
        default:
            movies_info_viewer_scene();
        }
    }
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

    // while (true)
    // {
    //     // info.update();
    //     bn::core::update();
    // }
}
