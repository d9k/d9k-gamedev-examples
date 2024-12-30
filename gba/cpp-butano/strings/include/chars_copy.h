#ifndef CHARS_COPY_H
#define CHARS_COPY_H

#include <cstring>

char* chars_copy(const char* sourceChars) {
    int sourceCharsLength = std::strlen(sourceChars);
    char* result = new char[sourceCharsLength + 1];
    std::strncpy(result, sourceChars, sourceCharsLength + 1);
    result[sourceCharsLength] = 0;
    return result;
}

#endif // CHARS_COPY_H