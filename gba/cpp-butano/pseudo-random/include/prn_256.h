#ifndef PRN_256_H
#define PRN_256_H
#include "bn_common.h"
#include "prn_data_256.h"
#include "bn_log.h"
#include <bitset>

namespace prn_256
{
    inline constexpr int SEED_LIMIT = 1 << (8+7);
    inline constexpr int POS_LIMIT = 1 << 8;

    struct POS_AND_SHIFT {
        uint8_t pos;
        uint8_t shift;
    };

    POS_AND_SHIFT seed_to_pos_and_shift(int seed);

    class Generator
    {
    public:
        uint8_t pos;
        uint8_t shift;

        Generator(int seed);

        uint8_t generate();
    };
}

#endif
