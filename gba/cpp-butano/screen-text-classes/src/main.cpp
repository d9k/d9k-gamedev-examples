#define BN_CFG_LOG_MAX_SIZE 1024
#define M_PI 3.14159265358979323846

#include "../hw/include/bn_hw_log.h"
#include <algorithm>
#include <cstring>
#include <sstream>
#include "bn_assert.h"
#include "bn_core.h"
#include "bn_log.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_sprite_ptr.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"
#include "bn_log.h"
#include "bn_string.h"
#include "chars_copy.h"
#include "common_fixed_8x16_sprite_font.h"
#include "chars_pointer_copy_wrapper.h"
#include "screen_text/abstract_block.h"
#include "screen_text/align.h"
#include "screen_text/rows_composer.h"
#include "screen_text/title.h"
#include "screen_text/caption_value_pair.h"
#include "screen_text/scrollable_block.h"

namespace
{
    constexpr bn::fixed text_y_inc = 14;
    constexpr bn::fixed text_y_limit = (bn::display::height() / 2) - text_y_inc;
    constexpr int rows_composer_first_row_cy_shift = -bn::display::height() / 2 + 16 / 2;
    constexpr int rows_composer_line_height = 18;
    constexpr int scrollable_block_row_height = 14;
    constexpr int key_value_pair_cx_shift = 30;
    constexpr int scrollable_block_window_rows = 6;
    constexpr int scrollable_block_window_columns = 26;
    constexpr int scrollable_block_scroll_vertical_delta = scrollable_block_window_rows - 1;
    constexpr int rows_composer_bottom_row_index = 8;
    constexpr int cx_position_at_right_border = bn::display::width() / 2 - 8;
    constexpr int cx_position_at_left_border = -bn::display::width() / 2 + 8;
    constexpr const char *bottom_line_text_default = "(START: go to next scene)";

    constexpr const char *long_text = "Hear the voice of the Bard! Who Present, Past, & Future sees Whose ears have heard The Holy Word, That walk'd among the ancient trees. Calling the lapsed Soul And weeping in the evening dew; That might control. The starry pole; And fallen fallen light renew! O Earth O Earth return! Arise from out the dewy grass; Night is worn, And the morn Rises from the slumbrous mass. Turn away no more: Why wilt thou turn away The starry floor The watery shore Is given thee till the break of day. / William Blake";

    void scrollable_blocks_classes_scene()
    {
        bn::core::update();

        bn::sprite_text_generator text_generator(common::fixed_8x16_sprite_font);

        screen_text::RowsComposer<128, 32> rows_composer(&text_generator, rows_composer_line_height);
        rows_composer.first_row_cy_shift = rows_composer_first_row_cy_shift;

        screen_text::Title title("Scrollable blocks classes");

        screen_text::Title position("", screen_text::ALIGN_RIGHT, true);
        position.custom_margin_with_last_block = 8;
        position.cx_shift = cx_position_at_right_border - 10;
        bn::string<32> position_string;
        bn::ostringstream position_string_stream(position_string);

        // position.cx_shift = x_position_at_right_border;

        screen_text::ScrollableBlock scrollable_block(long_text, scrollable_block_window_rows, scrollable_block_window_columns);

        scrollable_block.cx_shift = cx_position_at_left_border;
        // scrollable_block.cx_shift = -bn::display::width() + 16 / 2;

        scrollable_block.scroll_vertical_delta = scrollable_block_scroll_vertical_delta;
        scrollable_block.custom_margin_with_last_block = 2;
        scrollable_block.custom_row_height = scrollable_block_row_height;

        rows_composer.add_block(&title);
        rows_composer.add_block(&scrollable_block);
        rows_composer.add_block(&position);

        screen_text::Title bottom_title(bottom_line_text_default);
        bottom_title.custom_margin_with_last_block = 8;
        rows_composer.add_block(&bottom_title, rows_composer_bottom_row_index);

        while (!bn::keypad::start_pressed())
        {
            bn::core::update();

            if (bn::keypad::left_pressed() || bn::keypad::up_pressed())
            {
                scrollable_block.dec_scroll_vertical_current();
                rows_composer.reset();
            }
            if (bn::keypad::right_pressed() || bn::keypad::down_pressed())
            {
                scrollable_block.inc_scroll_vertical_current();
                rows_composer.reset();
            }

            position_string.assign("Scroll position: ");
            position_string_stream << scrollable_block.get_scroll_vertical_current();
            position_string_stream << "/";
            position_string_stream << scrollable_block.get_scroll_vertical_max();
            position.text = position_string.c_str();
            rows_composer.rerender();
        }
    }

    void caption_value_pairs_classes_scene()
    {
        bn::core::update();

        uint64_t frame_number = 1;

        bn::sprite_text_generator text_generator(common::fixed_8x16_sprite_font);

        screen_text::RowsComposer<64, 32> rows_composer(&text_generator, rows_composer_line_height);
        rows_composer.first_row_cy_shift = rows_composer_first_row_cy_shift;

        screen_text::Title title("Caption/value pairs classes");
        screen_text::CaptionValuePair frames_counter("frame");
        frames_counter.cx_shift = key_value_pair_cx_shift;

        rows_composer.add_block(&title);
        rows_composer.add_block(&frames_counter);

        screen_text::Title bottom_title(bottom_line_text_default);
        rows_composer.add_block(&bottom_title, rows_composer_bottom_row_index);

        while (!bn::keypad::start_pressed())
        {
            bn::core::update();
            frames_counter.dynamic_value = bn::to_string<16>(frame_number).c_str();
            rows_composer.rerender();
            frame_number++;
        }
    }

    void titles_classes_scene()
    {
        bn::core::update();
        // screen_text::AbstractBlock ablock = screen_text::AbstractBlock(3);

        // BN_LOG("AbstractBlock: rows num: ", ablock.get_rows_count());

        bn::sprite_text_generator text_generator(common::fixed_8x16_sprite_font);

        screen_text::RowsComposer<64, 32> rows_composer(&text_generator, rows_composer_line_height);
        rows_composer.first_row_cy_shift = rows_composer_first_row_cy_shift;

        screen_text::Title title("Titles classes example");

        screen_text::Title left("Left alignment", screen_text::ALIGN_LEFT);

        screen_text::Title left_shifted("Left shifted", screen_text::ALIGN_LEFT);
        left_shifted.cx_shift = cx_position_at_left_border;

        screen_text::Title left_shifted_longer("Left shifted longer", screen_text::ALIGN_LEFT);
        left_shifted_longer.cx_shift = cx_position_at_left_border;

        screen_text::Title right("Right alignment", screen_text::ALIGN_RIGHT);

        screen_text::Title right_shifted("Right shifted", screen_text::ALIGN_RIGHT);
        right_shifted.cx_shift = cx_position_at_right_border;

        screen_text::Title right_shifted_longer("Right shifted longer", screen_text::ALIGN_RIGHT);
        right_shifted_longer.cx_shift = cx_position_at_right_border;

        rows_composer.add_block(&title);
        rows_composer.add_block(&left);
        rows_composer.add_block(&left_shifted);
        rows_composer.add_block(&left_shifted_longer);
        rows_composer.add_block(&right);
        rows_composer.add_block(&right_shifted);
        rows_composer.add_block(&right_shifted_longer);

        screen_text::Title bottom_title(bottom_line_text_default);
        rows_composer.add_block(&bottom_title, rows_composer_bottom_row_index);

        while (!bn::keypad::start_pressed())
        {
            bn::core::update();
            rows_composer.rerender();
        }
    }

    void default_text_scene()
    {
        bn::core::update();

        bn::sprite_text_generator text_generator(common::fixed_8x16_sprite_font);
        text_generator.set_center_alignment();

        bn::vector<bn::sprite_ptr, 32> text_sprites;
        text_generator.generate(0, -text_y_limit, "Default procedural example", text_sprites);
        text_generator.generate(0, text_y_limit, bottom_line_text_default, text_sprites);

        while (!bn::keypad::start_pressed())
        {
            bn::core::update();
        }
    }
}

int main()
{
    bn::core::init();

    BN_LOG("BN_CFG_LOG_MAX_SIZE: ", BN_CFG_LOG_MAX_SIZE);

    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    while (true)
    {
        scrollable_blocks_classes_scene();
        caption_value_pairs_classes_scene();
        titles_classes_scene();
        default_text_scene();
    }
}
