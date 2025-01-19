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
#include "bn_fixed.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_sprite_ptr.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"
#include "bn_log.h"
#include "bn_string.h"
#include "chars_copy.h"
#include "common_variable_8x16_sprite_font.h"
#include "chars_pointer_copy_wrapper.h"
#include "chars_from_int.h"
#include "string_from_fixed.h"

using namespace std::string_literals;

namespace
{
    constexpr bn::fixed text_y_inc = 14;
    constexpr bn::fixed text_y_limit = (bn::display::height() / 2) - text_y_inc;

    class Person {
    public:
        CharsPointerCopyWrapper name{"John"};
    };

    void test_std_string()
    {
        BN_LOG("\n\n# Test std::string\n");

        std::string test_std_string = "std::string";
        BN_LOG("test_std_string() : initial: ", test_std_string.c_str());

        test_std_string.assign("another std::string");
        BN_LOG("test_std_string() : after reassign: ", test_std_string.c_str());

        std::string testStdString3 = "std::string"s + " test "s + "string"s + " concat"s;
        BN_LOG("test_std_string() : concat: ", testStdString3.c_str());
    }

    void test_chars()
    {
        BN_LOG("\n\n# Test chars\n");

        const char *sourceChars = "const char*";

        char targetChars[100] = "some long string hello world";
        char targetChars2[100] = "some long string hello world";
        char targetChars3[100] = "some long string hello world";
        char targetChars4[100] = "some long string hello world";

        std::strcpy(targetChars, sourceChars);
        int sourceCharsLength = std::strlen(sourceChars);
        std::strncpy(targetChars2, sourceChars, sourceCharsLength);
        std::strncpy(targetChars3, sourceChars, sourceCharsLength);
        targetChars3[sourceCharsLength] = 0;
        std::strncpy(targetChars4, sourceChars, sourceCharsLength + 1);

        char* charsCopyResult = chars_copy(sourceChars);
        char* charsCopyPartialResult = chars_copy(sourceChars, 7);

        const char *concatChars = "These are "
                                  "const "
                                  "strings";

        BN_LOG("test_chars() sourceCharsLength: ", sourceCharsLength);
        BN_LOG("test_chars() test copy 1: strcpy: ", targetChars);
        BN_LOG("test_chars() test copy 2: strncpy: ", targetChars2);
        BN_LOG("test_chars() test copy 3: strncpy with manual 0 end: ", targetChars3);
        BN_LOG("test_chars() test copy 4: strncpy with length + 1: ", targetChars4);
        BN_LOG("test_chars() test copy 5: chars_copy() result: ", charsCopyResult);
        BN_LOG("test_chars() test copy 6: chars_copy(, 7) result: ", charsCopyPartialResult);

        BN_LOG("test_chars() test concat: ", concatChars);

        delete[] charsCopyPartialResult;
        delete[] charsCopyResult;

        // char* chars_a = "Mood: good";
        // char* chars_b = "Mood when raining: bad";
        // // char* chars_to_search = ":";
        // char char_to_search = ':';
        // int search_in_only_these_first_chars = 6;

        // char* chars_b_search_result = std::find(chars_b, chars_b + search_in_only_these_first_chars, char_to_search);

        // int chars_a_search_position = chars_a_search_result - chars_a;
        // int chars_b_search_position = chars_b_search_result - chars_b;

        // BN_LOG("test_chars() chars a search postion: ", chars_a_search_position);
        // (not found)
        // BN_LOG("test_chars() chars b search postion: ", chars_b_search_position);
    }

    void test_bn_string()
    {
        BN_LOG("\n\n# Test bn::string\n");
        const char *sourceChars = "char*";

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

        const char *string_beginning = "Hello";
        int string_beginning_length = std::strlen(string_beginning);
        const char *string_a = "Hello, world!";
        const char *string_b = "Hi, everyone!";

        bn::string_view string_a_beginning_view = bn::string_view(string_a, string_beginning_length);
        bn::string_view string_b_beginning_view = bn::string_view(string_b, string_beginning_length);

        BN_LOG("test_bn_string_view() string_beginning: ", string_beginning);
        BN_LOG("test_bn_string_view() string_a_beginning_view: ", string_a_beginning_view);

        // Note that full data!
        BN_LOG("test_bn_string_view() string_a_beginning_view: data: ", string_a_beginning_view.data());
        BN_LOG("test_bn_string_view() string_a_beginning_view chars_copy(data): ", chars_copy(string_a_beginning_view));
        BN_LOG("test_bn_string_view() string_b_beginning_view: ", string_b_beginning_view);

        BN_LOG("test_bn_string_view() string_a_beginning_view == string_beginning: ", string_a_beginning_view == string_beginning);
        BN_LOG("test_bn_string_view() string_b_beginning_view == string_beginning: ", string_b_beginning_view == string_beginning);

        bn::string_view string_a_view = bn::string_view(string_a);
        bn::string_view string_b_view = bn::string_view(string_b);

        BN_LOG("test_bn_string_view() string_a_view.starts_with(string_beginning): ", string_a_view.starts_with(string_beginning));
        BN_LOG("test_bn_string_view() string_b_view.starts_with(string_beginning): ", string_b_view.starts_with(string_beginning));

        bn::string_view string_a_end_view = bn::string_view(string_a + string_beginning_length);
        BN_LOG("test_bn_string_view() string_a_end_view: ", string_a_end_view);

        const char *chars_a = "Mood: good";
        const char *chars_b = "Mood when raining: bad";
        const char *chars_to_search = ":";
        int search_in_only_these_first_chars = 6;

        bn::string_view chars_a_only_first_view = bn::string_view(chars_a, search_in_only_these_first_chars);
        bn::string_view chars_b_only_first_view = bn::string_view(chars_b, search_in_only_these_first_chars);

        int chars_a_only_first_search_result = chars_a_only_first_view.find(chars_to_search);
        int chars_b_only_first_search_result = chars_b_only_first_view.find(chars_to_search);

        BN_LOG("test_bn_string_view(): search \"", chars_to_search, "\" in only first ", search_in_only_these_first_chars, " chars of \"", chars_a, "\": ", chars_a_only_first_search_result);
        BN_LOG("test_bn_string_view(): search \"", chars_to_search, "\" in only first ", search_in_only_these_first_chars, " chars of \"", chars_b, "\": ", chars_b_only_first_search_result);
    }

    void test_sprintf()
    {
        BN_LOG("\n\n# Test sprintf\n");

        char buf[256];

        sprintf(buf, "Hello, %s! %d", "world", 123456);

        // TODO: %f built with DevKitARM corrupts memory (Allocation failed. Size in bytes: 4)
        // sprintf(buf, "Hello, %s! %d %.5f", "world", 123456, M_PI);

        BN_LOG("test_sprintf(): ", buf);
    }

    void test_object_with_chars()
    {
        BN_LOG("\n\n# Test object with chars\n");

        Person person;

        BN_LOG("test_object_with_chars(): before rename: person.name.get_chars(): ", person.name.get_chars());

        person.name.set_chars("Tom");

        BN_LOG("test_object_with_chars(): after rename, before person.name.get_chars() call");
        BN_LOG("test_object_with_chars(): after rename: person.name.get_chars(): ", person.name.get_chars());

        CharsPointerCopyWrapper local_chars_pointer_copy_wrapper = person.name;
        BN_LOG("local chars pointer copy wrapper: ", local_chars_pointer_copy_wrapper.get_chars());

        CharsPointerCopyWrapper local_chars_pointer_copy_wrapper_copy = CharsPointerCopyWrapper(person.name);
        BN_LOG("local chars pointer copy wrapper copy: ", local_chars_pointer_copy_wrapper.get_chars());
    }

    void test_std_string_stream() {
        BN_LOG("\n\n# Test std string stream\n");
        // std::string myString;
        std::ostringstream stringStream; // = std::stringstream(myString);
        const char* userName = "Admin";
        int id = 400;

        // TODO: doesn't work with DevKitARM
        // stringStream << "Hi, " << userName << " with id=" << id << "!";
        // const std::string &tmp = stringStream.str();
        // const char* chars = tmp.c_str();
        // BN_LOG(chars);
    }

    void test_chars_from_data()
    {
        BN_LOG("\n\n# Test chars from data\n");
        int int_1 = 123;
        int int_2 = 45;
        const char * chars_from_int_by_bn_string_1 = bn::to_string<16>(int_1).c_str();
        const char * chars_from_int_by_bn_string_2 = bn::to_string<16>(int_2).c_str();

        const char * chars_from_int_by_bn_string_copy_1 = chars_copy(bn::to_string<16>(int_1));
        const char * chars_from_int_by_bn_string_copy_2 = chars_copy(bn::to_string<16>(int_2));
        // bn::to_string<16>(45).c_str();
        // const char* chars_to_int_result_1 = chars

        bn::fixed_t<20> pi_fixed(M_PI);
        bn::string<64> pi_fixed_string = bn::to_string<64>(pi_fixed);
        const char * chars_from_fixed_float_by_bn_string_copy = chars_copy(bn::to_string<64>(pi_fixed));
        const char *chars_from_fixed = chars_copy(string_from_fixed<64>(pi_fixed));
        const char *chars_from_fixed_precision_10 = chars_copy(string_from_fixed<64>(pi_fixed, 10));

        // fail:
        BN_LOG("chars from int by bn::string, 1: ", chars_from_int_by_bn_string_1);
        BN_LOG("chars from int by bn::string, 2: ", chars_from_int_by_bn_string_2);

        BN_LOG("chars from int by bn::string + copy, 1: ", chars_from_int_by_bn_string_copy_1);
        BN_LOG("chars from int by bn::string + copy, 2: ", chars_from_int_by_bn_string_copy_2);

        BN_LOG("pi_fixed: ", pi_fixed);
        BN_LOG("pi_fixed_string: ", pi_fixed_string);
        BN_LOG("chars from fixed by bn::string + copy, 2: ", chars_from_fixed_float_by_bn_string_copy);

        BN_LOG("chars from fixed: ", chars_from_fixed);

        // TODO wrong digits!
        // 3.14159265358979323846
        // 3.141357421
        BN_LOG("chars from fixed (precision 10): ", chars_from_fixed_precision_10);

        // BN_LOG(string_from_fixed<64>(pi_fixed, 10));

        delete[] chars_from_fixed_precision_10;
        delete[] chars_from_fixed;
        delete[] chars_from_fixed_float_by_bn_string_copy;
        delete[] chars_from_int_by_bn_string_copy_2;
        delete[] chars_from_int_by_bn_string_copy_1;

        // const char* chars_from_int_1 = chars_from_int<16>(int_1).data();
        // const char* chars_from_int_2 = chars_from_int<16>(int_2).data();

        // TODO fail
        // BN_LOG("chars to int, 1: ", chars_from_int_1);
        // BN_LOG("chars to int, 2: ", chars_from_int_2);
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


    void test_assert_will_break_run()
    {
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
    test_object_with_chars();
    test_std_string_stream();
    test_chars_from_data();
    // test_assert_will_break_run();

    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    while (true)
    {
        text_scene();
        bn::core::update();
    }
}
