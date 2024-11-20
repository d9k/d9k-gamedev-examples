/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_sprite_ptr.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"
#include "bn_log.h"
#include "bn_string.h"
#include "bn_timer.h"
#include "bn_timers.h"
#include "bn_memory.h"

#include "common_variable_8x16_sprite_font.h"

namespace
{
    constexpr int text_y_inc = 14;
    constexpr bn::fixed text_y_limit_f = (bn::display::height() / 2) - text_y_inc;
    constexpr int text_y_limit = text_y_limit_f.integer();
    constexpr int x_dynamic_padding = 8;

    int frames_approximate_count = 0;
    int seed = 0;

    void text_scene()
    {
        // int a = 2'147'483'647;

        // [Fundamental types - cppreference.com](https://en.cppreference.com/w/cpp/language/types)
        // overflow 32 bit signed nt
        // int b = 2'147'483'649;
        // int c = -1;

        bn::timer timer;
        // uint64_t timer_total_ticks = 0;
        // __UINT64_TYPE__ timer_total_ticks = 0;
        // __uint64_t timer_total_ticks = 0;
        __fsblkcnt_t timer_total_ticks = 0;

        bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);

        bn::vector<bn::sprite_ptr, 32> text_sprites;
        bn::vector<bn::sprite_ptr, 32> dynamic_text_sprites;
        text_generator.set_center_alignment();
        text_generator.generate(0, -text_y_limit, "Text", text_sprites);

        int text_frames_approximate_y = -text_y_limit + text_y_inc;
        int text_frames_y = -text_y_limit + 2 * text_y_inc;
        int text_ram_y = -text_y_limit + 4 * text_y_inc;

        text_generator.set_right_alignment();
        text_generator.generate(0, text_frames_approximate_y, "frames approx.:", text_sprites);

        text_generator.set_right_alignment();
        text_generator.generate(0, text_frames_y, "frames:", text_sprites);

        // BN_LOG("a:", a, ", b:", b, ", c:", c);

        text_generator.set_right_alignment();
        text_generator.generate(0, text_ram_y, "ram:", text_sprites);

        text_generator.set_center_alignment();
        text_generator.generate(0, text_y_limit, "START: go to next scene", text_sprites);

        while (!bn::keypad::start_pressed())
        {
            frames_approximate_count++;
            dynamic_text_sprites.clear();
            text_generator.set_left_alignment();

            timer_total_ticks += timer.elapsed_ticks_with_restart();

            uint64_t frames_count = timer_total_ticks / bn::timers::ticks_per_frame();
            uint64_t seconds = timer_total_ticks / bn::timers::ticks_per_second();

            bn::string<32> frames_approximate_text;
            bn::ostringstream frames_approximate_text_steam(frames_approximate_text);
            frames_approximate_text_steam.append(frames_approximate_count);

            bn::string<32> frames_text;
            bn::ostringstream frames_text_stream(frames_text);
            frames_text_stream.append(frames_count);

            text_generator.set_left_alignment();
            text_generator.generate(x_dynamic_padding, text_frames_approximate_y, frames_approximate_text, dynamic_text_sprites);

            text_generator.set_left_alignment();
            text_generator.generate(x_dynamic_padding, text_frames_y, frames_text, dynamic_text_sprites);

            bn::core::update();
        }
    }

    void text_alignment_scene()
    {
        bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);

        bn::vector<bn::sprite_ptr, 32> text_sprites;
        text_generator.set_center_alignment();
        text_generator.generate(0, -text_y_limit, "Text alignment", text_sprites);

        text_generator.set_left_alignment();
        text_generator.generate(0, -text_y_inc, "Left", text_sprites);

        text_generator.set_center_alignment();
        text_generator.generate(0, 0, "Center", text_sprites);

        text_generator.set_right_alignment();
        text_generator.generate(0, text_y_inc, "Right", text_sprites);

        text_generator.set_center_alignment();
        text_generator.generate(0, text_y_limit, "START: go to next scene", text_sprites);

        while (!bn::keypad::start_pressed())
        {
            bn::core::update();
        }
    }

    void sprite_per_character_text_scene()
    {
        bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
        text_generator.set_center_alignment();

        bn::vector<bn::sprite_ptr, 32> text_sprites;
        text_generator.generate(0, -text_y_limit, "Sprite per character text", text_sprites);

        bn::vector<bn::sprite_ptr, 32> character_sprites;
        text_generator.set_one_sprite_per_character(true);
        text_generator.generate(0, 0, "ONE SPRITE PER CHARACTER", character_sprites);

        text_generator.set_one_sprite_per_character(false);
        text_generator.generate(0, text_y_limit, "START: go to next scene", text_sprites);

        bn::fixed angle;
        bn::fixed angle_inc = 6;

        int ram_ew_available = bn::memory::available_alloc_ewram();
        // int ram_ew_available = bn::memory::available_alloc_ewram();

        while (!bn::keypad::start_pressed())
        {
            angle += angle_inc;

            if (angle >= 360)
            {
                angle -= 360;
            }

            bn::fixed local_angle = angle;

            for (bn::sprite_ptr &character_sprite : character_sprites)
            {
                local_angle += angle_inc;

                if (local_angle >= 360)
                {
                    local_angle -= 360;
                }

                character_sprite.set_y(bn::degrees_lut_sin(local_angle) * 16);
            }

            bn::core::update();
        }
    }

    void utf8_text_scene()
    {
        bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
        text_generator.set_center_alignment();

        bn::vector<bn::sprite_ptr, 32> text_sprites;
        text_generator.generate(0, -text_y_limit, "UTF-8 text", text_sprites);
        text_generator.generate(0, 0, "UTF-8 characters: áéñÁÉÑ", text_sprites);
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

    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    while (true)
    {
        text_scene();
        bn::core::update();

        text_alignment_scene();
        bn::core::update();

        sprite_per_character_text_scene();
        bn::core::update();

        utf8_text_scene();
        bn::core::update();
    }
}
