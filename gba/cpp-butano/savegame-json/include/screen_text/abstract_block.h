#ifndef SCREEN_TEXT_ABSTRACT_BLOCK_H
#define SCREEN_TEXT_ABSTRACT_BLOCK_H

#include "bn_log.h"
#include "bn_vector.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "screen_text/sprites_vector.h"

namespace screen_text
{
    class AbstractBlock
    {
    public:
        int current_row_index = 0;
        bn::sprite_text_generator *custom_text_generator = nullptr;
        int custom_margin_with_last_block = 0;
        int custom_row_cx_shift = 0;
        int custom_row_height = 0;
        int rendered_block_cy_shift = 0;
        int rendered_block_height = 0;
        int rows_count = 1;

        /** comes from RowsComposer */
        int row_cx_shift = 0;
        int row_cy_shift = 0;

        bool _static_rendered = false;

        AbstractBlock(int rowsCount = 1)
        {
            rows_count = rowsCount;
        }

        virtual ~AbstractBlock()
        {
        }

        virtual int get_block_type();

        virtual int get_rows_count()
        {
            return rows_count;
        }

        void rerender(
            SpritesVector *staticSprites,
            SpritesVector *dynamicSprites,
            bn::sprite_text_generator *defaultTextGenerator)
        {
            // BN_LOG("screen_text::AbstractBlock: rerender(): cy_shift: ", cy_shift);
            if (!_static_rendered)
            {
                // BN_LOG("screen_text::AbstractBlock: rerender(): calling render_static_to_sprites");
                render_static_to_sprites(staticSprites, defaultTextGenerator);
            }
            else
            {
                // BN_LOG("screen_text::AbstractBlock: rerender(): NOT calling render_static_to_sprites");
            }
            render_dynamic_to_sprites(dynamicSprites, defaultTextGenerator);
        }

        virtual void render_static_to_sprites(SpritesVector *staticSprites, bn::sprite_text_generator *defaultTextGenerator)
        {
            process_render_static_to_sprites(staticSprites, defaultTextGenerator);
            _static_rendered = true;
        }

        virtual void render_dynamic_to_sprites(SpritesVector *dynamicSprites, bn::sprite_text_generator *defaultTextGenerator)
        {
            process_render_dynamic_to_sprites(dynamicSprites, defaultTextGenerator);
        }

        void reset()
        {
            _static_rendered = false;
        }

        bn::sprite_text_generator *get_current_text_generator(bn::sprite_text_generator *defaultTextGenerator)
        {
            if (this->custom_text_generator != nullptr)
            {
                return this->custom_text_generator;
            }
            return defaultTextGenerator;
        }

        virtual void process_render_static_to_sprites(SpritesVector *, bn::sprite_text_generator *)
        {
        }

        virtual void process_render_dynamic_to_sprites(SpritesVector *, bn::sprite_text_generator *)
        {
        }

        int get_current_row_cx_shift()
        {
            return custom_row_cx_shift != 0 ? custom_row_cx_shift : row_cx_shift;
        }
    };

    typedef AbstractBlock *AbstractBlockPtr;
}

#endif // SCREEN_TEXT_ABSTRACT_BLOCK_H