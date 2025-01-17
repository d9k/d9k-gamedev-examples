#ifndef SCREEN_TEXT_TITLE_H
#define SCREEN_TEXT_TITLE_H

#include "bn_vector.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "screen_text/sprites_vector.h"
#include "screen_text/abstract_block.h"

namespace screen_text
{
    class Title : public AbstractBlock
    {
    public:
        bn::sprite_text_generator::alignment_type alignment;
        const char *text;

        bool _dynamic;

        Title(
            const char *staticText,
            bn::sprite_text_generator::alignment_type _alignment = bn::sprite_text_generator::alignment_type::CENTER,
            bool dynamic = false) : AbstractBlock(1)
        {
            text = staticText;
            alignment = _alignment;
            _dynamic = dynamic;
        }

        void _render_to_sprites(SpritesVector *sprites, bn::sprite_text_generator *defaultTextGenerator)
        {
            bn::sprite_text_generator *text_generator = get_current_text_generator(defaultTextGenerator);
            text_generator->set_alignment(alignment);
            text_generator->generate(cx_shift, cy_shift, text, *sprites);
        }

        void process_render_static_to_sprites(SpritesVector *staticSprites, bn::sprite_text_generator *defaultTextGenerator) override
        {
            // BN_LOG("screen_text::StaticTitle: process_render_static_to_sprites(): cy_shift: ", cy_shift, ", text: ", static_text);
            if (!_dynamic)
            {
                _render_to_sprites(staticSprites, defaultTextGenerator);
            }
        }

        void process_render_dynamic_to_sprites(SpritesVector *dynamicSprites, bn::sprite_text_generator *defaultTextGenerator) override
        {
            if (_dynamic)
            {
                _render_to_sprites(dynamicSprites, defaultTextGenerator);
            }
        }
    };
}

#endif // SCREEN_TEXT_TITLE_H