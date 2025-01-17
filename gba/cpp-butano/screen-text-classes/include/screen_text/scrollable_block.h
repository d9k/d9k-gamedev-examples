#ifndef SCREEN_TEXT_SCROLLABLE_BLOCK_H
#define SCREEN_TEXT_SCROLLABLE_BLOCK_H

#include <algorithm>
#include "screen_text/abstract_block.h"
#include "screen_text/align.h"
#include "chars_copy.h"

namespace screen_text
{
    class ScrollableBlock : public AbstractBlock
    {
    public:
        const char *static_text;
        int static_text_length;
        int scroll_vertical_delta = 1;

        int _window_columns_count;
        int _window_row_current = 0;
        int _scroll_vertical_current = 0;
        int _scroll_vertical_max;

        ScrollableBlock(const char *staticText, int windowRowsCount, int windowColumnsCount)
        {
            rows_count = windowRowsCount;
            _window_columns_count = windowColumnsCount;
            set_static_text(staticText);
        }

        void set_static_text(const char *staticText)
        {
            static_text = staticText;
            static_text_length = std::strlen(static_text);
            _scroll_vertical_current = 0;
            int window_chars_count = rows_count * _window_columns_count;
            int chars_count_over_window = std::max(static_text_length - window_chars_count, 0);
            _scroll_vertical_max = chars_count_over_window / _window_columns_count + 1;
        }

        int get_scroll_vertical_max()
        {
            return _scroll_vertical_max;
        }

        int get_scroll_vertical_current()
        {
            return _scroll_vertical_current;
        }

        void set_scroll_vertical_current(int new_vertical_scroll)
        {
            if (new_vertical_scroll < 0)
            {
                new_vertical_scroll = 0;
            }
            if (new_vertical_scroll > _scroll_vertical_max)
            {
                new_vertical_scroll = _scroll_vertical_max;
            }
            _scroll_vertical_current = new_vertical_scroll;
        }

        void inc_scroll_vertical_current()
        {
            set_scroll_vertical_current(_scroll_vertical_current + scroll_vertical_delta);
        }

        void dec_scroll_vertical_current()
        {
            set_scroll_vertical_current(_scroll_vertical_current - scroll_vertical_delta);
        }

        virtual void render_static_to_sprites(SpritesVector *staticSprites, bn::sprite_text_generator *defaultTextGenerator)
        {
            bn::sprite_text_generator *text_generator = get_current_text_generator(defaultTextGenerator);

            int window_begin_char_pos = (_scroll_vertical_current + _window_row_current) * _window_columns_count;

            const char *static_text_in_window = chars_copy(static_text + window_begin_char_pos, _window_columns_count);

            text_generator->set_alignment(screen_text::ALIGN_LEFT);
            text_generator->generate(cx_shift, cy_shift, static_text_in_window, *staticSprites);

            _window_row_current++;

            if (_window_row_current >= rows_count)
            {
                _window_row_current = 0;
                _static_rendered = true;
            }
        }
    };
}

#endif // SCREEN_TEXT_SCROLLABLE_BLOCK_H