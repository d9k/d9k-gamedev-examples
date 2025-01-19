#ifndef CHARS_COPY_H
#define CHARS_COPY_H

#include <cstring>
#include "bn_string.h"

char *chars_copy(const char *sourceChars, int firstNCharsOnly = 0)
{
    // BN_LOG("chars_copy: 100");
    int sourceCharsLength = firstNCharsOnly;
    if (!sourceCharsLength)
    {
        // BN_LOG("chars_copy: 200");
        sourceCharsLength = std::strlen(sourceChars);
    }
    // BN_LOG("chars_copy: 300");
    char *result = new char[sourceCharsLength + 1];
    // BN_LOG("chars_copy: 400");
    std::strncpy(result, sourceChars, sourceCharsLength + 1);
    // BN_LOG("chars_copy: 500");
    result[sourceCharsLength] = 0;
    // BN_LOG("chars_copy: 600");
    return result;
}

char *chars_copy(bn::string_view sourceStringView, int firstNCharsOnly = 0)
{
    int stringViewCharsLength = firstNCharsOnly;
    if (!stringViewCharsLength)
    {
        stringViewCharsLength = sourceStringView.size();
    }
    return chars_copy(sourceStringView.data(), stringViewCharsLength);
}

#endif // CHARS_COPY_Hendif // CHARS_COPY_HH