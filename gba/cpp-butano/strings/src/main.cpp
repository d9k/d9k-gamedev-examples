#define BN_CFG_LOG_MAX_SIZE 1024

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

#include "common_variable_8x16_sprite_font.h"

using namespace std::string_literals;

namespace
{
    constexpr bn::fixed text_y_inc = 14;
    constexpr bn::fixed text_y_limit = (bn::display::height() / 2) - text_y_inc;

    void testStdString() {
        BN_LOG("\n\n# Test std::string\n");
        // concatenation fail with error undefined reference to `std::__throw_length_error(char const*)'
        // std::string testStdString = "std::string"s + " test "s + "string"s + " concat"s;
        std::string testStdString = "std::string";
        // char *testConcat = "test " + "string" + " concat";
        BN_LOG("testStdString(): ", testStdString.c_str());

        // TODO: linker error
        // testStdString.assign("another std::string");
        // BN_LOG("testStdString (2): ", testStdString.c_str());
    }

    void testBnStringView() {
        BN_LOG("\n\n# Test bn::string_view\n");
        bn::string_view *v = new bn::string_view("bn::string_view test");
        BN_LOG("testBnStringView() 1: ", *v, ", size:", v->size());
        delete v;

        v = new bn::string_view("another bn::string_view test");
        BN_LOG("testBnStringView() 2: ", *v, ", size:", v->size());
        delete v;
    }

    void text_scene()
    {
        bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
        text_generator.set_center_alignment();

        bn::vector<bn::sprite_ptr, 32> text_sprites;
        text_generator.generate(0, -text_y_limit, "Strings example. See debug console", text_sprites);
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

    BN_LOG("BN_CFG_LOG_MAX_SIZE: ", BN_CFG_LOG_MAX_SIZE);

    testStdString();
    testBnStringView();

    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    while(true)
    {
        text_scene();
        bn::core::update();

    }
}
