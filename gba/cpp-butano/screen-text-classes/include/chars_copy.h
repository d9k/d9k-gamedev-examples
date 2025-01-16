#ifndef CHARS_COPY_H
#define CHARS_COPY_H

#include <cstring>
#include "bn_string.h"

char *chars_copy(const char *sourceChars, int firstNCharsOnly = 0)
{
    int length_to_copy = firstNCharsOnly;
    int source_chars_length = std::strlen(sourceChars);

    if (!length_to_copy || length_to_copy > source_chars_length)
    {
        length_to_copy = source_chars_length;
    }

    char *result = new char[length_to_copy + 1];
    std::strncpy(result, sourceChars, length_to_copy + 1);
    result[length_to_copy] = 0;
    return result;
}

char *chars_copy(bn::string_view sourceStringView, int firstNCharsOnly = 0)
{
    int length_to_copy = firstNCharsOnly;

    int source_chars_length = sourceStringView.size();
    if (!length_to_copy || length_to_copy > source_chars_length)
    {
        length_to_copy = source_chars_length;
    }
    return chars_copy(sourceStringView.data(), length_to_copy);
}

#endif // CHARS_COPY_H