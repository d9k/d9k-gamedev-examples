#ifndef STRING_FROM_FIXED_H
#define STRING_FROM_FIXED_H

#include "bn_fixed.h"
#include "bn_string.h"

// TODO wrong digits
template <int TemplateCharsMaxLength>
bn::string<TemplateCharsMaxLength> string_from_fixed(bn::fixed fixed, int precision = 6) {
    bn::string<TemplateCharsMaxLength> result;
    bn::ostringstream result_stream(result);
    result_stream.set_precision(precision);
    result_stream << fixed;
    return result;
}

#endif // STRING_FROM_FIXED_H