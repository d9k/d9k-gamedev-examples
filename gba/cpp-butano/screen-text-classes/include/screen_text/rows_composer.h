#ifndef SCREEN_TEXT_ROWS_COMPOSER_H
#define SCREEN_TEXT_ROWS_COMPOSER_H

#include "screen_text/abstract_block.h"
#include "bn_sprite_text_generator.h"

namespace screen_text
{
    template <int TemplateStaticSpritesMaxCount, int TemplateDynamicSpritesMaxCount>
    class RowsComposer
    {
        static constexpr const int MAX_ROWS = 32;
        bn::vector<bn::sprite_ptr, TemplateStaticSpritesMaxCount> static_sprites;
        bn::vector<bn::sprite_ptr, TemplateDynamicSpritesMaxCount> dynamic_sprites;

        int first_row_cy_shift = 0;
        int cx_shift = 0;
        int row_height;
        int next_new_row_index = 0;
        bn::sprite_text_generator text_generator;
        AbstractBlockPtr _row_num_to_block_object[MAX_ROWS];

        RowsComposer(bn::sprite_text_generator textGenerator, int rowHeight)
        {
            this->text_generator = textGenerator;
            this->row_height = rowHeight;
            delete_rows();
        }

        void add_block(AbstractBlock *block)
        {
            int rows_count = block->get_rows_count();

            for (int i = next_new_row_index; i < next_new_row_index + rows_count; i++) {
                if (_row_num_to_block_object[i] != NULL) {
                    bn::string<64> error_msg_no_place = "No place for new block with ";
                    error_msg_no_place += bn::to_string<16>(rows_count);
                    error_msg_no_place += ", next_ ";
                    BN_ASSERT(false, error_msg_no_place);
                    return;
                }
                _row_num_to_block_object[i] = block;
            }
            next_new_row_index += rows_count;
        }

        void delete_rows()
        {
            for (int i = 0; i < MAX_ROWS; i++)
            {
                _row_num_to_block_object[i] = NULL;
            }
            next_new_row_index = 0;
        }

        void rerender() {
            for (int i = 0; i < MAX_ROWS; i++) {
                if (_row_num_to_block_object[i] != NULL) {
                    _row_num_to_block_object[i]->rerender(&static_sprites, &dynamic_sprites);
                }
            }
        }

        void reset() {
            for (int i = 0; i < MAX_ROWS; i++) {
                if (_row_num_to_block_object[i] != NULL) {
                    _row_num_to_block_object[i]->reset();
                }
            }
        }
    };
}

#endif // SCREEN_TEXT_ROWS_COMPOSER_H