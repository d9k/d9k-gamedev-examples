#ifndef FAKE_STD_THROW_LENGTH_ERROR_H
#define FAKE_STD_THROW_LENGTH_ERROR_H

#include "bn_log.h"

/** https://forum.arduino.cc/t/arduino-due-warning-std-__throw_length_error-char-const/308515 */
namespace std {
  void __throw_length_error(char const*) {
    BN_LOG("std custom length error");
  }

  void __throw_bad_array_new_length() {
    BN_LOG("std custom __throw_bad_array_new_length");
  }
}

#endif // FAKE_STD_THROW_LENGTH_ERROR_H