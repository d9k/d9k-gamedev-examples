#ifndef MOVIE_H
#define MOVIE_H

#include <string>
#include "bn_core.h"
#include "bn_string.h"
#include "chars_pointer_copy_wrapper.h"

class Movie {
public:
    CharsPointerCopyWrapper id = CharsPointerCopyWrapper("[no id]");
    CharsPointerCopyWrapper title = CharsPointerCopyWrapper("[no title]");
    CharsPointerCopyWrapper plot_text = CharsPointerCopyWrapper("[no plot text]");
    int year = 0;
};

#endif