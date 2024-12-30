#ifndef SEMVER_FROM_CHARS_H
#define SEMVER_FROM_CHARS_H

#include "bn_assert.h"
#include "semver_by_Neargye/semver.h"

semver::version semver_from_chars(const char *version_chars)
{
    semver::version return_value = semver::version();
    int version_chars_len = std::strlen(version_chars);
    semver::from_chars_result conversion_result =  return_value.from_chars(version_chars, version_chars + version_chars_len);

    BN_ASSERT(conversion_result.ec != std::errc::invalid_argument, "semver_from_chars(): invalid argument");

    return return_value;
}

#endif // SEMVER_FROM_CHARS_H