#ifndef MOVIES_H
#define MOVIES_H

#include <any>
#include <vector>
#include "movie.h"

// typedef std::vector<Movie*> Movies;
typedef bn::vector<Movie, 128> Movies;
// typedef std::vector<std::any> Movies;
// typedef std::any Movies;

#endif // MOVIES_H