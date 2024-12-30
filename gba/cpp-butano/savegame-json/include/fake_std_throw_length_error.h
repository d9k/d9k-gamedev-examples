#ifndef FAKE_STD_THROW_LENGTH_ERROR_H
#define FAKE_STD_THROW_LENGTH_ERROR_H

#include "bn_assert.h"

/** https://forum.arduino.cc/t/arduino-due-warning-std-__throw_length_error-char-const/308515 */
namespace std {
  void __throw_length_error(char const*) {
    BN_ASSERT(false, "std custom length error");
    exit(1);
  }

  void __throw_bad_array_new_length() {
    BN_ASSERT("std custom __throw_bad_array_new_length");
    exit(1);
  }
}

#endif // FAKE_STD_THROW_LENGTH_ERROR_H