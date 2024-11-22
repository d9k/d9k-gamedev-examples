#include "prn_256.h"

namespace prn_256
{
    constexpr int SEED_LIMIT = 1 << (8+7);

    POS_AND_SHIFT seed_to_pos_and_shift(int seed)
    {
        uint16_t _seed = seed % SEED_LIMIT;
        uint8_t pos = 0;
        uint8_t shift_multiplier = 0;

        std::bitset<16> seed_bitset(_seed);
        std::bitset<8> pos_bitset(0);
        std::bitset<8> shift_multiplier_bitset(0);

        for (int seed_bit_index = 0; seed_bit_index < 16; seed_bit_index++)
        {
            int seed_bit_digit = seed_bitset[seed_bit_index];
            BN_LOG("seed_base2[", seed_bit_index, "]: ", seed_bit_digit);

            if (seed_bit_index % 2 == 1)
            {
                int shift_multiplier_bit_index = seed_bit_index >> 1;
                shift_multiplier_bitset[shift_multiplier_bit_index] = seed_bit_digit;
            }
            else
            {
                int pos_bit_index = seed_bit_index >> 1;
                pos_bitset[pos_bit_index] = seed_bit_digit;
            }
        }

        pos = pos_bitset.to_ulong();
        shift_multiplier = shift_multiplier_bitset.to_ulong();
        uint8_t shift = ((shift_multiplier << 1) + 1) % 256;
        BN_LOG("pos: ", pos);
        BN_LOG("shift multiplier: ", shift_multiplier);
        BN_LOG("shift: ", shift);

        POS_AND_SHIFT result = {
            pos,
            shift,
        };

        return result;
    }

    class prn_generator_256
    {
        void construct(int seed)
        {
        }

        void generate()
        {
        }

        // int set
    };
}