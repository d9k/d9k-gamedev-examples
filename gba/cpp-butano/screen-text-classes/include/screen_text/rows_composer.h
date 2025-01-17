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

        int first_row_cy_shift = 0;
        int next_new_row_index = 0;
        int row_height;
        bn::sprite_text_generator *text_generator;
        AbstractBlockPtr _row_num_to_block_object[MAX_ROWS];

        RowsComposer(bn::sprite_text_generator *textGenerator, int rowHeight)
        {
            this->text_generator = textGenerator;
            this->row_height = rowHeight;
            delete_rows();

            BN_LOG("screen_text::RowsComposer: rerender(): blocks array addr:", &_row_num_to_block_object);
        }

        void add_block(AbstractBlock *block, int position_index = -1)
        {
            int rows_count = block->get_rows_count();

            int new_row_index = position_index != -1 ? position_index : next_new_row_index;

            for (int i = new_row_index; i < new_row_index + rows_count; i++)
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

            if (position_index == -1)
            {
                next_new_row_index += rows_count;
            }
        }

        void delete_rows()
        {
            for (int i = 0; i < MAX_ROWS; i++)
            {
                _row_num_to_block_object[i] = nullptr;
            }
            next_new_row_index = 0;
        }

        bool current_row_in_the_same_block_with_previous_row(int i)
        {
            AbstractBlock *current_block = _row_num_to_block_object[i];
            int last_i = i - 1;
            if (last_i < 0)
            {
                return false;
            }
            AbstractBlock *last_block = _row_num_to_block_object[last_i];
            if (last_block == nullptr)
            {
                return false;
            }
            return last_block == current_block;
        }

        void rerender()
        {
            dynamic_sprites.clear();
            int cy_shift = first_row_cy_shift;
            int current_row_height;
            int margin_with_last_block;

            for (int i = 0; i < MAX_ROWS; i++)
            {
                AbstractBlock *block = _row_num_to_block_object[i];

                // BN_LOG("screen_text::RowsComposer: rerender(): i:", i, ", block: ", block);
                current_row_height = row_height;
                margin_with_last_block = 0;

                if (block != nullptr)
                {
                    // BN_LOG("screen_text::RowsComposer: rerender(): rendering #", i, " block");
                    if (!current_row_in_the_same_block_with_previous_row(i))
                    {
                        margin_with_last_block = block->custom_margin_with_last_block;
                    }

                    block->set_cy_shift(cy_shift + margin_with_last_block);
                    block->rerender(&static_sprites, &dynamic_sprites, text_generator);

                    if (block->custom_row_height)
                    {
                        current_row_height = block->custom_row_height;
                    }
                }
                cy_shift += current_row_height + margin_with_last_block;
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