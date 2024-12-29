#ifndef CHARS_POINTER_WRAPPER_H
#define CHARS_POINTER_WRAPPER_H

#include "chars_copy.h"

class CharsPointerWrapper
{
public:
    char *_chars = NULL;
    char *_default_chars = NULL;

    CharsPointerWrapper(char *default_value = "")
    {
        _default_chars = default_value;
    }

    ~CharsPointerWrapper()
    {
        if (_chars != NULL)
        {
            delete[] _chars;
        }
    }

    const char *get_chars()
    {
        if (_chars == NULL)
        {
            return _default_chars;
        }
        return _chars;
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

#endif // CHARS_POINTER_WRAPPER_H