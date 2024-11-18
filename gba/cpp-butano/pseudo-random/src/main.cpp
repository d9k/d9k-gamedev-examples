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

#include "common_variable_8x16_sprite_font.h"

namespace
{
    constexpr bn::fixed text_y_inc = 14;
    constexpr bn::fixed text_y_limit = (bn::display::height() / 2) - text_y_inc;

    void text_scene()
    {
        bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
        text_generator.set_center_alignment();

        bn::vector<bn::sprite_ptr, 32> text_sprites;
        text_generator.generate(0, -text_y_limit, "Text", text_sprites);
        text_generator.generate(0, text_y_limit, "START: go to next scene", text_sprites);

        while(! bn::keypad::start_pressed())
        {
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

        while(! bn::keypad::start_pressed())
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

        while(! bn::keypad::start_pressed())
        {
            angle += angle_inc;

            if(angle >= 360)
            {
                angle -= 360;
            }

            bn::fixed local_angle = angle;

            for(bn::sprite_ptr& character_sprite : character_sprites)
            {
                local_angle += angle_inc;

                if(local_angle >= 360)
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

        while(! bn::keypad::start_pressed())
        {
            bn::core::update();
        }
    }
}

int main()
{
    bn::core::init();

    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    while(true)
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
