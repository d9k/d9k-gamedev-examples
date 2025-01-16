#ifndef SCREEN_TEXT_ROWS_COMPOSER_H
#define SCREEN_TEXT_ROWS_COMPOSER_H

#include "bn_array.h"
#include "bn_log.h"
#include "bn_sprite_text_generator.h"
#include "screen_text/abstract_block.h"

namespace screen_text
{
    template <int TemplateStaticSpritesMaxCount, int TemplateDynamicSpritesMaxCount>
    class RowsComposer
    {
    public:
        static constexpr const int MAX_ROWS = 32;
        bn::vector<bn::sprite_ptr, TemplateStaticSpritesMaxCount> static_sprites;
        bn::vector<bn::sprite_ptr, TemplateDynamicSpritesMaxCount> dynamic_sprites;

        // int cx_shift = 0;
        int first_row_cy_shift = 0;
        int next_new_row_index = 0;
        int row_height;
        bn::sprite_text_generator *text_generator;
        AbstractBlockPtr _row_num_to_block_object[MAX_ROWS];
        // bn::array<AbstractBlock*, MAX_ROWS> _row_num_to_block_object;

        RowsComposer(bn::sprite_text_generator *textGenerator, int rowHeight)
        {
            this->text_generator = textGenerator;
            this->row_height = rowHeight;
            delete_rows();

            BN_LOG("screen_text::RowsComposer: rerender(): blocks array addr:", &_row_num_to_block_object);
        }

        void add_block(AbstractBlock *block)
        {
            int rows_count = block->get_rows_count();

            for (int i = next_new_row_index; i < next_new_row_index + rows_count; i++)
            {
                if (_row_num_to_block_object[i] != nullptr)
                {
                    bn::string<64> error_msg_no_place = "No place for new block with ";
                    error_msg_no_place += bn::to_string<16>(rows_count);
                    error_msg_no_place += ", next_ ";
                    BN_ASSERT(false, error_msg_no_place);
                    return;
                }
                BN_LOG("screen_text::RowsComposer: add_block(): adding ", block, " at index ", i);
                _row_num_to_block_object[i] = block;
            }
            next_new_row_index += rows_count;
        }

        void delete_rows()
        {
            for (int i = 0; i < MAX_ROWS; i++)
            {
                _row_num_to_block_object[i] = nullptr;
            }
            next_new_row_index = 0;
        }

        // void __attribute__((optimize("O0"))) rerender()
        void rerender()
        {
            dynamic_sprites.clear();
            int cy_shift = first_row_cy_shift;

            // for (int i = 0; i < MAX_ROWS; i++) {
            //     AbstractBlock *block = _row_num_to_block_object[i];
            //     BN_LOG("screen_text::RowsComposer: rerender(): i:", i, ", block: ", block);
            //     // TODO BUG! Last title not drawn! Fixed if uncomment:
            //     // BN_LOG("screen_text::RowsComposer: rerender(): i:", i, ", block: ", block);
            //     // if (block != NULL) {
            //         // block->set_cy_shift(cy_shift);
            //         // block->rerender(&static_sprites, &dynamic_sprites, text_generator);
            //     // }
            //     // cy_shift += row_height;
            // }

            for (int i = 0; i < MAX_ROWS; i++)
            {
                AbstractBlock *block = _row_num_to_block_object[i];
                // TODO BUG! Last title not drawn! Fixed if uncomment:
                BN_LOG("screen_text::RowsComposer: rerender(): i:", i, ", block: ", block);

                if (block != nullptr)
                {
                    // if (_row_num_to_block_object[i] != nullptr) {
                    // AbstractBlock *block = _row_num_to_block_object[i];
                    BN_LOG("screen_text::RowsComposer: rerender(): rendering #", i, " block");
                    block->set_cy_shift(cy_shift);
                    // block->set_cx_shift(cx_shift);
                    block->rerender(&static_sprites, &dynamic_sprites, text_generator);
                }
                cy_shift += row_height;
            }
        }

        void reset()
        {
            static_sprites.clear();
            for (int i = 0; i < MAX_ROWS; i++)
            {
                AbstractBlock *block = _row_num_to_block_object[i];
                if (block != nullptr)
                {
                    block->reset();
                }
            }
        }
    };
}

#endif // SCREEN_TEXT_ROWS_COMPOSER_H