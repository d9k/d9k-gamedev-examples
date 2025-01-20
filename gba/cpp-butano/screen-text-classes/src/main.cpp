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
#include "screen_text/rows_composer.h"
#include "screen_text/title.h"
#include "screen_text/caption_value_pair.h"
#include "screen_text/scrollable_block.h"

#include "bn_sprite_items_arrow_down.h"
#include "bn_sprite_items_arrow_up.h"

namespace
{
    constexpr bn::fixed text_y_inc = 14;
    constexpr bn::fixed text_y_limit = (bn::display::height() / 2) - text_y_inc;
    constexpr int rows_composer_first_row_cy_shift = -bn::display::height() / 2 + 16 / 2;
    constexpr int rows_composer_line_height = 18;
    constexpr int scrollable_block_row_height = 14;
    constexpr int key_value_pair_cx_shift_default = 40;
    constexpr int key_value_pair_cx_shift_custom = 25;
    constexpr int scrollable_block_window_rows = 6;
    constexpr int scrollable_block_window_columns = 26;
    constexpr int scrollable_block_scroll_vertical_delta = scrollable_block_window_rows - 1;
    constexpr int rows_composer_bottom_row_index = 8;
    constexpr int cx_position_at_right_border = bn::display::width() / 2 - 8;
    constexpr int cx_position_at_left_border = -bn::display::width() / 2 + 8;
    constexpr const char *bottom_line_text_default = "(START: go to next scene)";

    constexpr const char *long_text = "Hear the voice of the Bard! Who Present, Past, & Future sees Whose ears have heard The Holy Word, That walk'd among the ancient trees. Calling the lapsed Soul And weeping in the evening dew; That might control. The starry pole; And fallen fallen light renew! O Earth O Earth return! Arise from out the dewy grass; Night is worn, And the morn Rises from the slumbrous mass. Turn away no more: Why wilt thou turn away The starry floor The watery shore Is given thee till the break of day. / William Blake";

    inline void arrow_sprites_update_visible(
        bn::sprite_ptr *arrow_up,
        bn::sprite_ptr *arrow_down,
        screen_text::ScrollableBlock *scrollable_block)
    {
        arrow_up->set_visible(scrollable_block->can_scroll_up());
        arrow_down->set_visible(scrollable_block->can_scroll_down());
    }

    void scrollable_blocks_classes_scene()
    {
        bn::core::update();

        bool first_render = true;

        bn::sprite_ptr arrow_down = bn::sprite_items::arrow_down.create_sprite(
            cx_position_at_right_border - 4,
            0);
        arrow_down.set_visible(false);

        bn::sprite_ptr arrow_up = bn::sprite_items::arrow_up.create_sprite(
            cx_position_at_right_border - 4,
            0);
        arrow_up.set_visible(false);

        bn::sprite_text_generator text_generator(common::fixed_8x16_sprite_font);

        screen_text::RowsComposer<128, 32> rows_composer(&text_generator, rows_composer_line_height);
        rows_composer.first_row_cy_shift = rows_composer_first_row_cy_shift;

        screen_text::Title title("Scrollable blocks classes");

        screen_text::Title position("", screen_text::ALIGN_RIGHT, true);
        position.custom_margin_with_last_block = 8;
        position.row_cx_shift = cx_position_at_right_border - 10;
        bn::string<32> position_string;
        bn::ostringstream position_string_stream(position_string);

        screen_text::ScrollableBlock scrollable_block(long_text, scrollable_block_window_rows, scrollable_block_window_columns);

        scrollable_block.row_cx_shift = cx_position_at_left_border - 2;

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
                arrow_sprites_update_visible(&arrow_up, &arrow_down, &scrollable_block);
                rows_composer.reset();
            }
            if (bn::keypad::right_pressed() || bn::keypad::down_pressed())
            {
                scrollable_block.inc_scroll_vertical_current();
                arrow_sprites_update_visible(&arrow_up, &arrow_down, &scrollable_block);
                rows_composer.reset();
            }

            position_string.assign("Scroll position: ");
            position_string_stream << scrollable_block.get_scroll_vertical_current();
            position_string_stream << "/";
            position_string_stream << scrollable_block.get_scroll_vertical_max();
            position.chars = position_string.c_str();
            rows_composer.rerender();

            if (first_render)
            {
                first_render = false;
                arrow_up.set_y(scrollable_block.rendered_block_cy_shift);

                arrow_down.set_y(scrollable_block.rendered_block_cy_shift + scrollable_block.rendered_block_height - 16);

                arrow_sprites_update_visible(&arrow_up, &arrow_down, &scrollable_block);
            }
        }
    }

    void caption_value_pairs_classes_scene()
    {
        bn::core::update();

        uint64_t frame_number = 1;
        bn::fixed_t<20> pi_fixed(M_PI);

        bn::sprite_text_generator text_generator(common::fixed_8x16_sprite_font);

        screen_text::RowsComposer<64, 64> rows_composer(&text_generator, rows_composer_line_height);
        rows_composer.first_row_cy_shift = rows_composer_first_row_cy_shift;
        rows_composer.key_value_pair_cx_shift_default = key_value_pair_cx_shift_default;

        screen_text::Title title("Caption/value pairs classes");

        screen_text::CaptionValuePair frames_counter("frame");
        screen_text::CaptionValuePair pi_display("Pi");

        screen_text::CaptionValuePair custom_cx_shift_display("Custom cx shift");
        custom_cx_shift_display.custom_row_cx_shift = key_value_pair_cx_shift_custom;

        rows_composer.add_block(&title);
        rows_composer.add_block(&frames_counter);
        rows_composer.add_block(&pi_display);
        rows_composer.add_block(&custom_cx_shift_display);

        screen_text::Title bottom_title(bottom_line_text_default);
        rows_composer.add_block(&bottom_title, rows_composer_bottom_row_index);

        while (!bn::keypad::start_pressed())
        {
            frames_counter.dynamic_value.set_chars(bn::to_string<16>(frame_number));

            // char pi_display_chars[16];
            // char *pi_display_chars = new char[16];
            // TODO: %f built with DevKitARM corrupts memory (Allocation failed. Size in bytes: ...)
            // sprintf(pi_display_chars, "%.5f", M_PI);
            // pi_display.dynamic_value.set_chars(pi_display_chars);
            // delete pi_display_chars;

            pi_display.dynamic_value.set_chars(bn::to_string<64>(pi_fixed));

            custom_cx_shift_display.dynamic_value.set_chars(bn::to_string<16>(key_value_pair_cx_shift_custom));

            bn::core::update();
            rows_composer.rerender();

            frame_number++;
        }
    }

    void titles_classes_scene()
    {
        bn::core::update();

        bn::sprite_text_generator text_generator(common::fixed_8x16_sprite_font);

        screen_text::RowsComposer<64, 32> rows_composer(&text_generator, rows_composer_line_height);
        rows_composer.first_row_cy_shift = rows_composer_first_row_cy_shift;

        screen_text::Title title("Titles classes example");

        screen_text::Title left("Left alignment", screen_text::ALIGN_LEFT);

        screen_text::Title left_shifted("Left shifted", screen_text::ALIGN_LEFT);
        left_shifted.row_cx_shift = cx_position_at_left_border;

        screen_text::Title left_shifted_longer("Left shifted longer", screen_text::ALIGN_LEFT);
        left_shifted_longer.row_cx_shift = cx_position_at_left_border;

        screen_text::Title right("Right alignment", screen_text::ALIGN_RIGHT);

        screen_text::Title right_shifted("Right shifted", screen_text::ALIGN_RIGHT);
        right_shifted.row_cx_shift = cx_position_at_right_border;

        screen_text::Title right_shifted_longer("Right shifted longer", screen_text::ALIGN_RIGHT);
        right_shifted_longer.row_cx_shift = cx_position_at_right_border;

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
