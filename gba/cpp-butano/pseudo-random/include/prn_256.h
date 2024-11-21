#ifndef PRN_256_H
#define PRN_256_H

#include "bn_common.h"

#define PRN_256_SIZE 1024

namespace prn_256
{
  extern int max;
  extern int repeat;
  extern int size;
  extern BN_DATA_EWRAM int values[PRN_256_SIZE];
}

#endif