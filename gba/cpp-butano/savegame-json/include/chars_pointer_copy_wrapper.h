#ifndef CHARS_POINTER_COPY_WRAPPER_H
#define CHARS_POINTER_COPY_WRAPPER_H

#include "chars_copy.h"

class CharsPointerCopyWrapper
{
public:
    char *_chars = NULL;
    char *_default_chars = NULL;

    CharsPointerCopyWrapper(char *default_value = "")
    {
        _default_chars = default_value;
    }

    ~CharsPointerCopyWrapper()
    {
        if (_chars != NULL)
        {
            delete[] _chars;
        }
    }

    const char *get_chars()
    {
        if (_chars != NULL)
        {
            return _chars;
        }
        return _default_chars;
    }

    void set_chars(const char *value)
    {
        if (_chars != NULL)
        {
            delete[] _chars;
        }
        _chars = chars_copy(value);
    }
};

#endif // CHARS_POINTER_COPY_WRAPPER_H