#include "bn_core.h"
#include "bn_math.h"
// #include "bn_bitset.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_string.h"
#include "bn_sprite_ptr.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"
#include "bn_log.h"
#include "bn_string.h"
#include "bn_timer.h"
#include "bn_timers.h"
#include "bn_memory.h"
// #include "bit_ops.h"

// #include "prn_data_256.h"
#include "prn_256.h"

#include "common_variable_8x16_sprite_font.h"

namespace
{
    constexpr int text_y_inc = 14;
    constexpr bn::fixed text_y_limit_f = (bn::display::height() / 2) - text_y_inc;
    constexpr int text_y_limit = text_y_limit_f.integer();
    constexpr int x_static_padding = 24;
    constexpr int x_dynamic_padding = x_static_padding + 8;

    uint64_t frames_approximate_count = 0;
    int frame_in_second = 1;
    int seed = 0;
    int seed_inc_multiplier_last = 0;
    bn::string<32> text_seed_inc;

    void seed_increase(int seed_inc_multiplier)
    {
        if (!seed_inc_multiplier || seed_inc_multiplier == seed_inc_multiplier_last)
        {
            return;
        }

        int seed_inc = seed_inc_multiplier * frame_in_second;
        seed += seed_inc;

        text_seed_inc = "";
        bn::ostringstream text_seed_inc_stream(text_seed_inc);
        text_seed_inc_stream << frame_in_second;
        text_seed_inc_stream << " x ";
        text_seed_inc_stream << seed_inc_multiplier;
        text_seed_inc_stream << " = ";
        text_seed_inc_stream << seed_inc;
    }

    void scene_seed()
    {
        bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
        bn::vector<bn::sprite_ptr, 64> text_sprites;
        bn::vector<bn::sprite_ptr, 32> dynamic_text_sprites;

        bn::timer timer;
        uint64_t timer_total_ticks = 0;

        constexpr int text_header_y = -text_y_limit;
        constexpr int text_frames_approximate_y = -text_y_limit + text_y_inc;
        constexpr int text_frames_y = -text_y_limit + 2 * text_y_inc;
        constexpr int text_frame_in_second_y = -text_y_limit + 3 * text_y_inc;
        constexpr int text_ram_ew_y = -text_y_limit + 4 * text_y_inc;
        constexpr int text_ram_iw_y = -text_y_limit + 5 * text_y_inc;
        constexpr int text_seed_inc_y = -text_y_limit + 6 * text_y_inc;
        constexpr int text_seed_y = -text_y_limit + 7 * text_y_inc;
        constexpr int text_bottom_y = text_y_limit;

        text_generator.set_center_alignment();
        text_generator.generate(0, text_header_y, "Generating seed. Press any keys", text_sprites);
        text_generator.generate(0, text_bottom_y, "START: go to next scene", text_sprites);

        text_generator.set_right_alignment();
        text_generator.generate(x_static_padding, text_frames_approximate_y, "frames approx.:", text_sprites);
        text_generator.generate(x_static_padding, text_frame_in_second_y, "frame in second:", text_sprites);
        text_generator.generate(x_static_padding, text_frames_y, "frames:", text_sprites);
        text_generator.generate(x_static_padding, text_ram_ew_y, "EWRAM,static+alloc:", text_sprites);
        text_generator.generate(x_static_padding, text_ram_iw_y, "IWRAM,static+stack:", text_sprites);
        text_generator.generate(x_static_padding, text_seed_inc_y, "seed inc:", text_sprites);
        text_generator.generate(x_static_padding, text_seed_y, "seed:", text_sprites);

        while (!bn::keypad::start_pressed())
        {
            timer_total_ticks += timer.elapsed_ticks_with_restart();
            uint64_t frames_count = timer_total_ticks / bn::timers::ticks_per_frame();
            uint64_t seconds = timer_total_ticks / bn::timers::ticks_per_second();

            frames_approximate_count++;

            frame_in_second = frames_approximate_count % 60 + 1;

            // uint64_t ram_ew_free = bn::memory::available_alloc_ewram();
            uint64_t ram_ew_used_static = bn::memory::used_static_ewram();
            uint64_t ram_ew_used_alloc = bn::memory::used_alloc_ewram();
            uint64_t ram_iw_used_static = bn::memory::used_static_iwram();
            uint64_t ram_iw_used_stack = bn::memory::used_stack_iwram();
            int seed_inc_multiplier_new = 0;

            if (bn::keypad::left_pressed())
            {
                seed_inc_multiplier_new = 1;
            }
            if (bn::keypad::right_pressed())
            {
                seed_inc_multiplier_new = 2;
            }
            if (bn::keypad::down_pressed())
            {
                seed_inc_multiplier_new = 3;
            }
            if (bn::keypad::up_pressed())
            {
                seed_inc_multiplier_new = 4;
            }
            if (bn::keypad::a_pressed())
            {
                seed_inc_multiplier_new = 5;
            }
            if (bn::keypad::b_pressed())
            {
                seed_inc_multiplier_new = 6;
            }
            if (bn::keypad::r_pressed())
            {
                seed_inc_multiplier_new = 7;
            }
            if (bn::keypad::l_pressed())
            {
                seed_inc_multiplier_new = 8;
            }

            seed_increase(seed_inc_multiplier_new);

            bn::string<16> text_frames_approximate = bn::to_string<16>(frames_approximate_count);
            bn::string<16> text_frames = bn::to_string<16>(frames_count);

            bn::string<32> text_ram_ew_used = "";
            bn::ostringstream text_ram_ew_used_stream(text_ram_ew_used);
            text_ram_ew_used_stream << ram_ew_used_static;
            text_ram_ew_used_stream << " + ";
            text_ram_ew_used_stream << ram_ew_used_alloc;

            bn::string<32> text_ram_iw_used = "";
            bn::ostringstream text_ram_iw_used_stream(text_ram_iw_used);
            text_ram_iw_used_stream << ram_iw_used_static;
            text_ram_iw_used_stream << " + ";
            text_ram_iw_used_stream << ram_iw_used_stack;

            bn::string<16> text_frame_in_second = bn::to_string<16>(frame_in_second);
            bn::string<16> text_seed = bn::to_string<16>(seed);

            dynamic_text_sprites.clear();
            text_generator.set_left_alignment();
            text_generator.generate(x_dynamic_padding, text_frames_approximate_y, text_frames_approximate, dynamic_text_sprites);
            text_generator.generate(x_dynamic_padding, text_frames_y, text_frames, dynamic_text_sprites);
            text_generator.generate(x_dynamic_padding, text_ram_ew_y, text_ram_ew_used, dynamic_text_sprites);
            text_generator.generate(x_dynamic_padding, text_ram_iw_y, text_ram_iw_used, dynamic_text_sprites);
            text_generator.generate(x_dynamic_padding, text_frame_in_second_y, text_frame_in_second, dynamic_text_sprites);
            text_generator.generate(x_dynamic_padding, text_seed_inc_y, text_seed_inc, dynamic_text_sprites);
            text_generator.generate(x_dynamic_padding, text_seed_y, text_seed, dynamic_text_sprites);

            bn::core::update();
        }

        text_sprites.clear();
        dynamic_text_sprites.clear();
    }

    void scene_generate_numbers()
    {
        bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
        bn::vector<bn::sprite_ptr, 64> text_sprites;
        bn::vector<bn::sprite_ptr, 32> dynamic_text_sprites;

        constexpr int text_header_y = -text_y_limit;
        constexpr int text_bottom_y = text_y_limit;
        constexpr int text_seed_y = -text_y_limit + 2 * text_y_inc;
        constexpr int text_generator_pos_y = -text_y_limit + 3 * text_y_inc;
        constexpr int text_generator_shift_y = -text_y_limit + 4 * text_y_inc;
        constexpr int text_random_number_y = -text_y_limit + 6 * text_y_inc;

        text_generator.set_center_alignment();
        text_generator.generate(0, text_header_y, "Generating random number", text_sprites);
        text_generator.generate(0, text_bottom_y, "A: generate, START: next scene", text_sprites);

        text_generator.set_right_alignment();
        text_generator.generate(x_static_padding, text_seed_y, "seed:", text_sprites);
        text_generator.generate(x_static_padding, text_generator_pos_y, "generator pos:", text_sprites);
        text_generator.generate(x_static_padding, text_generator_shift_y, "generator shift:", text_sprites);
        text_generator.generate(x_static_padding, text_random_number_y, "pseudo random number:", text_sprites);

        prn_256::Generator prn256_generator(seed);

        int random_number = 0;

        while (!bn::keypad::start_pressed())
        {
            if (bn::keypad::a_pressed()) {
                random_number = prn256_generator.generate();
            }

            bn::string<16> text_seed = bn::to_string<16>(seed);
            bn::string<16> text_generator_pos = bn::to_string<16>(prn256_generator.pos);
            bn::string<16> text_generator_shift = bn::to_string<16>(prn256_generator.shift);
            bn::string<16> text_random_number = bn::to_string<8>(random_number);

            dynamic_text_sprites.clear();
            text_generator.set_left_alignment();
            text_generator.generate(x_dynamic_padding, text_seed_y, text_seed, dynamic_text_sprites);
            text_generator.generate(x_dynamic_padding, text_generator_pos_y, text_generator_pos, dynamic_text_sprites);
            text_generator.generate(x_dynamic_padding, text_generator_shift_y, text_generator_shift, dynamic_text_sprites);
            text_generator.generate(x_dynamic_padding, text_random_number_y, text_random_number, dynamic_text_sprites);

            bn::core::update();
        }

        text_sprites.clear();
        dynamic_text_sprites.clear();
    }

    void scene_utf8_text()
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

        text_sprites.clear();
    }
}

int main()
{
    bn::core::init();

    // [Fundamental types - cppreference.com](https://en.cppreference.com/w/cpp/language/types)
    // overflow 32 bit signed nt
    // int b = 2'147'483'649;
    // int a = 2'147'483'647;
    // int c = -1;
    // BN_LOG("a:", a, ", b:", b, ", c:", c);

    // BN_LOG("prn_data_256::values[3]: ", prn_data_256::values[3]);
    // BN_LOG("prn_data_256::max: ", prn_data_256::max);

    // BN_LOG("seed limit: ", prn_256::SEED_LIMIT);
    // // int _seed = 0b01101011;
    // int _seed = prn_256::SEED_LIMIT + 1;
    // prn_256::POS_AND_SHIFT pos_and_shift = prn_256::seed_to_pos_and_shift(_seed);
    // BN_LOG("result pos: ", pos_and_shift.pos);
    // BN_LOG("result shift: ", pos_and_shift.shift);

    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    while (true)
    {
        scene_seed();
        bn::core::update();

        scene_generate_numbers();
        bn::core::update();

        scene_utf8_text();
        bn::core::update();
    }
}
