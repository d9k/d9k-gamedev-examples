/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_blending.h"
#include "bn_sprite_palette_ptr.h"
#include "bn_sprite_tiles_ptr.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

#include "bn_sprite_items_flash_palette.h"
// #include "bn_sprite_items_blue_sprite.h"

namespace
{
    void colorize_sprite_pixel(bn::span<bn::tile> tile_span, int x, int y, uint8_t value) {
        int tile_index = (x / 8) + 2 * (y / 8);
        bn::tile& tile = tile_span[tile_index];
        uint32_t row = tile.data[y];

        int shift_amount = x * 4;
        row &= ~(0xF << shift_amount);
        row |= (value & 0xF) << shift_amount;

        tile.data[y] = row;
    }
}

int main()
{
    bn::core::init();

    alignas(4) bn::tile gfx_tiles[] = {
        {0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000},
        {0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000},
        {0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000},
        {0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000}
    };

    bn::sprite_tiles_item tiles_item = bn::sprite_tiles_item(gfx_tiles, bn::bpp_mode::BPP_4);
    bn::sprite_shape_size sprite_shape_size = bn::sprite_shape_size(16, 16);
    bn::sprite_tiles_ptr my_sprite_tiles_ptr = tiles_item.create_tiles();
    // bn::sprite_palette_ptr sprite_palette_ptr = bn::sprite_palette_items::ma_palette.create_palette();
    // bn::sprite_ptr blue_sprite = bn::sprite_items::blue_sprite.create_sprite(0, 0);
    // bn::sprite_palette_ptr sprite_palette_ptr = bn::sprite_items::blue_sprite.palette_item().create_palette();
    // bn::sprite_palette_ptr sprite_palette_ptr = bn::sprite_items::blue_sprite.create_palette();
    bn::sprite_palette_ptr sprite_palette_ptr = bn::sprite_items::flash_palette.palette_item().create_palette();
    bn::sprite_ptr logo_sprite_ptr = bn::sprite_ptr::create(0, 0, sprite_shape_size, my_sprite_tiles_ptr, sprite_palette_ptr);

    int x = 0;
    int y = 0;

    while(true) {
        // Fill the sprite pixel per pixel
        if(bn::keypad::a_held()) {
            if(y < 16) {
                colorize_sprite_pixel(gfx_tiles, x, y, 1);
                x+=1;
                if(x == 16) {
                    y+=1;
                    x=0;
                }
                my_sprite_tiles_ptr.reload_tiles_ref();
            }
        }
        bn::core::update();
    }
}
