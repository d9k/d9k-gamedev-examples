#ifndef SCREEN_TEXT_SPRITES_VECTOR_H
#define SCREEN_TEXT_SPRITES_VECTOR_H

#include "bn_vector.h"
#include "bn_sprite_ptr.h"

namespace screen_text {
    typedef bn::ivector<bn::sprite_ptr> SpritesVector;
}

#endif // SCREEN_TEXT_SPRITES_VECTOR_H