#ifndef SCREEN_TEXT_STATIC_TITLE_H
#define SCREEN_TEXT_STATIC_TITLE_H

#include "bn_vector.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "screen_text/sprites_vector.h"
#include "screen_text/abstract_block.h"

namespace screen_text
{
    class StaticTitle : public AbstractBlock
    {
    public:
        const char *static_text;
        bn::sprite_text_generator::alignment_type alignment;

        StaticTitle(
            const char *staticText,
            bn::sprite_text_generator::alignment_type _alignment = bn::sprite_text_generator::alignment_type::CENTER) : AbstractBlock(1)
        {
            static_text = staticText;
            alignment = _alignment;
        }

        void process_render_static_to_sprites(SpritesVector *staticSprites, bn::sprite_text_generator *defaultTextGenerator) override
        {
            // BN_LOG("screen_text::StaticTitle: process_render_static_to_sprites(): cy_shift: ", cy_shift, ", text: ", static_text);

            bn::sprite_text_generator *text_generator = get_current_text_generator(defaultTextGenerator);
            text_generator->set_alignment(alignment);
            text_generator->generate(cx_shift, cy_shift, static_text, *staticSprites);
        }
    };
}

#endif // SCREEN_TEXT_STATIC_TITLE_H