#define BN_CFG_LOG_MAX_SIZE 1024
// #define _USE_MATH_DEFINES
#define M_PI 3.14159265358979323846

/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

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
#include "screen_text/rows_composer.h"
#include "screen_text/static_title.h"

using namespace std::string_literals;

namespace
{
    constexpr bn::fixed text_y_inc = 14;
    constexpr bn::fixed text_y_limit = (bn::display::height() / 2) - text_y_inc;

    void titles_text_generator_scene()
    {
        bn::core::update();
        screen_text::AbstractBlock ablock = screen_text::AbstractBlock(3);

        BN_LOG("AbstractBlock: rows num: ", ablock.get_rows_count());

        bn::sprite_text_generator text_generator(common::fixed_8x16_sprite_font);

        screen_text::RowsComposer<64, 32> rows_composer(&text_generator, 18);
        rows_composer.first_row_cy_shift = -bn::display::height() / 2 + 16 / 2;

        screen_text::StaticTitle title("Titles text generator");

        screen_text::StaticTitle left("Left alignment", bn::sprite_text_generator::alignment_type::LEFT);

        screen_text::StaticTitle right("Right alignment", bn::sprite_text_generator::alignment_type::RIGHT);

        screen_text::StaticTitle row4("Row 4");

        rows_composer.add_block(&title);
        rows_composer.add_block(&left);
        rows_composer.add_block(&right);
        rows_composer.add_block(&row4);

        // rows_composer.rerender();

        while (!bn::keypad::start_pressed())
        {
            bn::core::update();
            rows_composer.rerender();
        }
        bn::core::update();
    }

    void default_text_scene()
    {
        bn::core::update();

        bn::sprite_text_generator text_generator(common::fixed_8x16_sprite_font);
        text_generator.set_center_alignment();

        bn::vector<bn::sprite_ptr, 32> text_sprites;
        text_generator.generate(0, -text_y_limit, "Screen text classes example", text_sprites);
        text_generator.generate(0, text_y_limit, "START: go to next scene", text_sprites);

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
        titles_text_generator_scene();
        default_text_scene();
    }
}
