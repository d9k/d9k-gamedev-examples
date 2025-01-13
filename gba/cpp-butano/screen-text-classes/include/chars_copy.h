#ifndef CHARS_COPY_H
#define CHARS_COPY_H

#include <cstring>
#include "bn_string.h"

char *chars_copy(const char *sourceChars, int firstNCharsOnly = 0)
{
    int sourceCharsLength = firstNCharsOnly;
    if (!sourceCharsLength)
    {
        sourceCharsLength = std::strlen(sourceChars);
    }
    char *result = new char[sourceCharsLength + 1];
    std::strncpy(result, sourceChars, sourceCharsLength + 1);
    result[sourceCharsLength] = 0;
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