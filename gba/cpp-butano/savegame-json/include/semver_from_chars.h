#ifndef SEMVER_FROM_CHARS_H
#define SEMVER_FROM_CHARS_H

#include "semver_by_Neargye/semver.h"

semver::version* semver_from_chars(char *version_chars)
{
    semver::version *result = new semver::version();
    int version_chars_len = std::strlen(version_chars);
    result->from_chars(version_chars, version_chars + version_chars_len);
    return result;
}

#endif // SEMVER_FROM_CHARS_H