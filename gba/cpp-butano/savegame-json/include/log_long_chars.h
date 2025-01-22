#ifndef LOG_LONG_CHARS_H
#define LOG_LONG_CHARS_H

#include <cstring>
#include "bn_core.h"

void log_long_chars(const char *longChars, int partMaxSize, const char *caption)
{
    int longCharsLen = std::strlen(longChars);
    int longCharsEnd = longCharsLen - 1;
    int partBegin = 0;
    int partLength = 0;
    int partEnd = -1;
    char *part = new char[partMaxSize + 1];

    do
    {
        partBegin = partEnd + 1;
        partLength = bn::min(partMaxSize, longCharsLen - partBegin);
        partEnd = partBegin + partLength - 1;

        char counter_text[64];
        sprintf(counter_text, "%d-%d/%d", partBegin, partEnd, longCharsLen - 1);

        std::strncpy(part, longChars + partBegin, partLength);
        // std::strncpy(part, longChars, partLength);
        part[partLength] = 0;

        BN_LOG(
            caption,
            ": ",
            counter_text,
            ":\n",
            part);
    } while (partEnd < longCharsEnd);

    delete[] part;
}

#endif // LOG_LONG_CHARS_H