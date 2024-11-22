#ifndef PRN_256_H
#define PRN_256_H
#include "bn_common.h"
#include "prn_data_256.h"
#include "bn_log.h"
#include <bitset>



namespace prn_256
{
    struct POS_AND_SHIFT {
        uint8_t index;
        uint8_t shift;
    };

    POS_AND_SHIFT seed_to_pos_and_shift(int seed);
}


#endif
