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

    CharsPointerCopyWrapper(const CharsPointerCopyWrapper &source) {
        CharsPointerCopyWrapper::set_fields_to_from(this, &source);
    }

    CharsPointerCopyWrapper& operator=(const CharsPointerCopyWrapper& other) = default;

    static void set_fields_to_from(CharsPointerCopyWrapper *target, const CharsPointerCopyWrapper *source)
    {
        // TODO test
        target->_default_chars = source->_default_chars;

        target->set_chars(source->_chars);
    }

    ~CharsPointerCopyWrapper()
    {
        _destruct_chars();
    }

    inline void _destruct_chars() {
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
        _destruct_chars();
        _chars = chars_copy(value);
    }
};

#endif // CHARS_POINTER_COPY_WRAPPER_H