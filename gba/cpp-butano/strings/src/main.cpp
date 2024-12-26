#define BN_CFG_LOG_MAX_SIZE 1024

/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include <cstring>
#include "bn_assert.h"
#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_sprite_ptr.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"
#include "bn_log.h"
#include "bn_string.h"
#include "fake_std_throw_length_error.h"

#include "common_variable_8x16_sprite_font.h"

using namespace std::string_literals;

namespace
{
    constexpr bn::fixed text_y_inc = 14;
    constexpr bn::fixed text_y_limit = (bn::display::height() / 2) - text_y_inc;

    void testStdString()
    {
        BN_LOG("\n\n# Test std::string\n");

        std::string testStdString = "std::string";
        // char *testConcat = "test " + "string" + " concat";
        BN_LOG("testStdString() test1 : ", testStdString.c_str());

        // TODO: linker error: undefined reference to `std::__cxx11::basic_string <char, std::char_traits<char>, std::allocator<char> >::_M_replace_cold`...
        testStdString.assign("another std::string");
        BN_LOG("testStdString() test2: ", testStdString.c_str());

        // Fixed with "fake_std_throw_length_error.h": concatenation fail with error undefined reference to `std::__throw_length_error(char const*)'
        // TODO: linker error: undefined reference to `std::__cxx11::basic_string <char, std::char_traits<char>, std::allocator<char> >::_M_replace_cold`...
        // std::string testStdString3 = "std::string"s + " test "s + "string"s + " concat"s;
        // BN_LOG("testStdString() test 3: ", testStdString3.c_str());
    }

    void testChars() {
        BN_LOG("\n\n# Test chars\n");

        const char* sourceChars = "const char*";

        char targetChars[100] = "some long string hello world";
        char targetChars2[100] = "some long string hello world";
        char targetChars3[100] = "some long string hello world";
        char targetChars4[100] = "some long string hello world";

        std::strcpy(targetChars, sourceChars);
        int sourceCharsLength = std::strlen(sourceChars);
        std::strncpy(targetChars2, sourceChars, sourceCharsLength);
        std::strncpy(targetChars3, sourceChars, sourceCharsLength);
        targetChars3[sourceCharsLength] = 0;

        char* concatChars = "These " "are " "strings";

        BN_LOG("testChars() test copy 1: strcpy: ", targetChars);
        BN_LOG("testChars() test copy 2: strncpy: ", targetChars2, ", length:", sourceCharsLength);
        BN_LOG("testChars() test copy 3: strncpy with manual 0 end: ", targetChars3, ", length:", sourceCharsLength);

        BN_LOG("testChars() test concat: ", concatChars);

        delete[] sourceChars;
    }


    void testBnString()
    {
        BN_LOG("\n\n# Test bn::string\n");
        char* sourceChars = "char*";

        bn::string<100> bnStringConstruct = sourceChars;

        bn::string<100> bnStringAssign = "some long string hello world";
        bnStringAssign.assign(sourceChars);

        BN_LOG("testBnString() test construct: ", bnStringConstruct);
        BN_LOG("testBnString() test assign: ", bnStringAssign);
    }

    void testBnStringView()
    {
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

        while (!bn::keypad::start_pressed())
        {
            bn::core::update();
        }
    }

    void testAssertWillBreakRun() {
        BN_LOG("\n\n# Test assert. This test will break run, comment it out!\n");
        int num = 123456;
        bn::string<64> errorMsg = "Test error message with number ";
        errorMsg += bn::to_string<16>(num);
        BN_ASSERT(false, errorMsg);
    }
}

int main()
{
    bn::core::init();

    BN_LOG("BN_CFG_LOG_MAX_SIZE: ", BN_CFG_LOG_MAX_SIZE);

    testStdString();
    testChars();
    testBnString();
    testBnStringView();
    // testAssertWillBreakRun();

    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    while (true)
    {
        text_scene();
        bn::core::update();
    }
}
