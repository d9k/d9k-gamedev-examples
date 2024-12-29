#define BN_CFG_LOG_MAX_SIZE 1024
// #define _USE_MATH_DEFINES
#define M_PI		3.14159265358979323846

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
// #include <math.h>

#include "common_variable_8x16_sprite_font.h"

using namespace std::string_literals;

namespace
{
    constexpr bn::fixed text_y_inc = 14;
    constexpr bn::fixed text_y_limit = (bn::display::height() / 2) - text_y_inc;

    void test_std_string()
    {
        BN_LOG("\n\n# Test std::string\n");

        std::string test_std_string = "std::string";
        // char *testConcat = "test " + "string" + " concat";
        BN_LOG("test_std_string() test1 : ", test_std_string.c_str());

        // TODO: linker error: undefined reference to `std::__cxx11::basic_string <char, std::char_traits<char>, std::allocator<char> >::_M_replace_cold`...
        // test_std_string.assign("another std::string");
        // BN_LOG("test_std_string() test2: ", test_std_string.c_str());

        // Fixed with "fake_std_throw_length_error.h": concatenation fail with error undefined reference to `std::__throw_length_error(char const*)'
        // TODO: linker error: undefined reference to `std::__cxx11::basic_string <char, std::char_traits<char>, std::allocator<char> >::_M_replace_cold`...
        // std::string testStdString3 = "std::string"s + " test "s + "string"s + " concat"s;
        // BN_LOG("test_std_string() test 3: ", testStdString3.c_str());
    }

    void test_chars() {
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

        BN_LOG("test_chars() test copy 1: strcpy: ", targetChars);
        BN_LOG("test_chars() test copy 2: strncpy: ", targetChars2, ", length:", sourceCharsLength);
        BN_LOG("test_chars() test copy 3: strncpy with manual 0 end: ", targetChars3, ", length:", sourceCharsLength);

        BN_LOG("test_chars() test concat: ", concatChars);

        delete[] sourceChars;
    }


    void test_bn_string()
    {
        BN_LOG("\n\n# Test bn::string\n");
        char* sourceChars = "char*";

        bn::string<100> bnStringConstruct = sourceChars;

        bn::string<100> bnStringAssign = "some long string hello world";
        bnStringAssign.assign(sourceChars);

        BN_LOG("test_bn_string() test construct: ", bnStringConstruct);
        BN_LOG("test_bn_string() test assign: ", bnStringAssign);
    }

    void test_bn_string_view()
    {
        BN_LOG("\n\n# Test bn::string_view\n");
        bn::string_view *v = new bn::string_view("bn::string_view test");
        BN_LOG("test_bn_string_view() 1: ", *v, ", size:", v->size());
        delete v;

        v = new bn::string_view("another bn::string_view test");
        BN_LOG("test_bn_string_view() 2: ", *v, ", size:", v->size());
        delete v;

        char * string_beginning = "Hello";
        int string_beginning_length = std::strlen(string_beginning);
        char * string_a = "Hello, world!";
        char * string_b = "Hi, everyone!";

        bn::string_view string_beginning_view = bn::string_view(string_beginning);
        bn::string_view string_a_beginning_view = bn::string_view(string_a, string_beginning_length);
        bn::string_view string_b_beginning_view = bn::string_view(string_b, string_beginning_length);

        BN_LOG("test_bn_string_view() string_a_beginning_view: ", string_a_beginning_view);
        BN_LOG("test_bn_string_view() string_b_beginning_view: ", string_b_beginning_view);

        BN_LOG("test_bn_string_view() string_a_beginning_view == string_beginning: ", string_a_beginning_view == string_beginning);
        BN_LOG("test_bn_string_view() string_b_beginning_view == string_beginning: ", string_b_beginning_view == string_beginning);

        bn::string_view string_a_end_view = bn::string_view(string_a + string_beginning_length);
        BN_LOG("test_bn_string_view() string_a_end_view: ", string_a_end_view);
    }

    void test_sprintf()
    {
        BN_LOG("\n\n# Test sprintf\n");

        char buf[256];
        sprintf(buf, "Hello, %s! %d %.5f", "world", 123456, M_PI);
        BN_LOG("test_sprintf(): ", buf);
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

    void test_assert_will_break_run() {
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

    test_std_string();
    test_chars();
    test_bn_string();
    test_bn_string_view();
    test_sprintf();
    // test_assert_will_break_run();

    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    while (true)
    {
        text_scene();
        bn::core::update();
    }
}
