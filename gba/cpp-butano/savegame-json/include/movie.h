#ifndef MOVIE_H
#define MOVIE_H

#include <string>
#include "bn_core.h"
#include "bn_string.h"
#include "chars_pointer_copy_wrapper.h"

class Movie
{
public:
    CharsPointerCopyWrapper chars_wrapper_id = CharsPointerCopyWrapper("[no id]");
    CharsPointerCopyWrapper title = CharsPointerCopyWrapper("[no title]");
    CharsPointerCopyWrapper plot_text = CharsPointerCopyWrapper("[no plot text]");
    int year = 0;

    void copy_fields_from(Movie *other)
    {
        chars_wrapper_id.copy_fields_from(&other->chars_wrapper_id);
        title.copy_fields_from(&other->title);
        plot_text.copy_fields_from(&other->plot_text);
        year = other->year;
    }
};

#endif