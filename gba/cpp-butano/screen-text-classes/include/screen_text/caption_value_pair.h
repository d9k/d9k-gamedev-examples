#ifndef SCREEN_TEXT_CAPTION_VALUE_PAIR_H
#define SCREEN_TEXT_CAPTION_VALUE_PAIR_H

#include "bn_string.h"
#include "bn_sprite_text_generator.h"
#include "bn_vector.h"
#include "screen_text/abstract_block.h"
#include "screen_text/align.h"

namespace screen_text
{
    class CaptionValuePair : public AbstractBlock
    {
    public:
        const char *static_caption;
        const char *dynamic_value;
        const char *separator = ": ";

        CaptionValuePair(
            const char *staticText)
        {
            static_caption = staticText;
        }

        void process_render_static_to_sprites(SpritesVector *staticSprites, bn::sprite_text_generator *defaultTextGenerator) override
        {
            // BN_LOG("screen_text::StaticTitle: process_render_static_to_sprites(): cy_shift: ", cy_shift, ", text: ", static_text);

            bn::sprite_text_generator *text_generator = get_current_text_generator(defaultTextGenerator);

            bn::string<32> static_caption_with_separator = "";
            bn::ostringstream static_caption_with_separator_stream(static_caption_with_separator);
            static_caption_with_separator_stream << static_caption;
            static_caption_with_separator_stream << separator;

            text_generator->set_alignment(screen_text::ALIGN_RIGHT);
            text_generator->generate(cx_shift, cy_shift, static_caption_with_separator, *staticSprites);
        }

        void process_render_dynamic_to_sprites(SpritesVector *dynamicSprites, bn::sprite_text_generator *defaultTextGenerator) override
        {
            bn::sprite_text_generator *text_generator = get_current_text_generator(defaultTextGenerator);

            text_generator->set_alignment(screen_text::ALIGN_LEFT);
            text_generator->generate(cx_shift, cy_shift, dynamic_value, *dynamicSprites);
        }
    };
}

#endif // SCREEN_TEXT_CAPTION_VALUE_PAIR_H