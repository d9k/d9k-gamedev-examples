#ifndef CHARS_POINTER_COPY_WRAPPER_H
#define CHARS_POINTER_COPY_WRAPPER_H

#include "chars_copy.h"

class CharsPointerCopyWrapper
{
public:
    char *_chars = NULL;
    const char *_default_chars;

    CharsPointerCopyWrapper(const char *default_value = "")
    {
        _default_chars = default_value;
    }

    // static CharsPointerCopyWrapper* copy_new(CharsPointerCopyWrapper other)
    // {
    //     CharsPointerCopyWrapper *result = new CharsPointerCopyWrapper(other._default_chars);
    //     // _default_chars = other._default_chars;
    //     result->set_chars(other.get_chars());
    //     return result;
    // }

    static void set_fields_from(CharsPointerCopyWrapper *target, CharsPointerCopyWrapper *source)
    {
        // CharsPointerCopyWrapper result = CharsPointerCopyWrapper(other._default_chars);
        // _default_chars = other._default_chars;
        // result.set_chars(other.get_chars());
        // return result;

        // TODO test
        target->_default_chars = source->_default_chars;

        target->set_chars(source->get_chars());
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

    template <typename T>
    void set_chars(const T value)
    {
        if (_chars != NULL)
        {
            delete[] _chars;
        }
        _chars = chars_copy(value);
    }
};

#endif // CHARS_POINTER_COPY_WRAPPER_H