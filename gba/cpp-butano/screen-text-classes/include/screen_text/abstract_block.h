#ifndef SCREEN_TEXT_ABSTRACT_BLOCK_H
#define SCREEN_TEXT_ABSTRACT_BLOCK_H

#include "bn_vector.h"
#include "bn_sprite_ptr.h"
#include "screen_text/sprites_vector.h"

namespace screen_text
{
    class AbstractBlock
    {
    public:
        bool _static_rendered;
        int rows_count = 1;
        int current_row_index = 0;

        AbstractBlock(int rowsCount)
        {
            rows_count = rowsCount;
        }

        virtual int get_rows_count()
        {
            return rows_count;
        }

        void render_static_to_sprites(SpritesVector *staticSprites)
        {
            process_render_static_to_sprites(staticSprites);
            _static_rendered = true;
        }

        void render_dynamic_to_sprites(SpritesVector *dynamicSprites)
        {
            process_render_dynamic_to_sprites(dynamicSprites);
        }

        void reset()
        {
            _static_rendered = false;
        }

        void rerender(SpritesVector *staticSprites, SpritesVector *dynamicSprites)
        {
            if (!_static_rendered)
            {
                render_static_to_sprites(staticSprites);
            }
            render_dynamic_to_sprites(dynamicSprites);
        }

        virtual void process_render_static_to_sprites(SpritesVector *)
        {
        }

        virtual void process_render_dynamic_to_sprites(SpritesVector *)
        {
        }
    };

    typedef AbstractBlock *AbstractBlockPtr;
}

#endif // SCREEN_TEXT_ABSTRACT_BLOCK_H