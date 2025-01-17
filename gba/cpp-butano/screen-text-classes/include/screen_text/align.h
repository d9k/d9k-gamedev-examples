#ifndef SCREEN_TEXT_ALIGN_H
#define SCREEN_TEXT_ALIGN_H

#include "bn_sprite_text_generator.h"

namespace screen_text
{
    typedef bn::sprite_text_generator::alignment_type Alignment;

    constexpr Alignment ALIGN_LEFT = bn::sprite_text_generator::alignment_type::LEFT;
    constexpr Alignment ALIGN_RIGHT = bn::sprite_text_generator::alignment_type::RIGHT;
    constexpr Alignment ALIGN_CENTER = bn::sprite_text_generator::alignment_type::CENTER;
}

#endif // SCREEN_TEXT_ALIGN_H