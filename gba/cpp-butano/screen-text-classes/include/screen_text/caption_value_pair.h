#ifndef SCREEN_TEXT_CAPTION_VALUE_PAIR_H
#define SCREEN_TEXT_CAPTION_VALUE_PAIR_H

// #include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_vector.h"

namespace screen_text
{
    class CaptionValuePair : public AbstractBlock
    {
    public:
        const char *static_text;
        const char *dynamic_text;

        CaptionValuePair(
            const char *staticText)
        {
            static_text = staticText;
        }

        void process_render_static_to_sprites(SpritesVector *staticSprites, bn::sprite_text_generator *defaultTextGenerator) override
        {
            // BN_LOG("screen_text::StaticTitle: process_render_static_to_sprites(): cy_shift: ", cy_shift, ", text: ", static_text);

            bn::sprite_text_generator *text_generator = get_current_text_generator(defaultTextGenerator);
            text_generator->set_alignment(bn::sprite_text_generator::alignment_type::RIGHT);
            text_generator->generate(cx_shift, cy_shift, static_text, *staticSprites);
        }

        void process_render_dynamic_to_sprites(SpritesVector *dynamicSprites, bn::sprite_text_generator *defaultTextGenerator) override
        {
            bn::sprite_text_generator *text_generator = get_current_text_generator(defaultTextGenerator);
            text_generator->set_alignment(bn::sprite_text_generator::alignment_type::LEFT);
            text_generator->generate(cx_shift, cy_shift, dynamic_text, *dynamicSprites);
        }
    };
}

#endif // SCREEN_TEXT_CAPTION_VALUE_PAIR_H