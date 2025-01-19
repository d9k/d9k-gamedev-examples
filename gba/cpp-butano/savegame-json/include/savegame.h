#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <vector>
#include "movie.h"
#include "movies.h"
#include "chars_copy.h"

class SaveGame
{
public:
    CharsPointerCopyWrapper selected_movie_id;
    int loads_count = 0;
    int _selected_movie_index = 0;

    Movies movies = {};

    void validate()
    {
        check_selected_movie_id();
    }

    void set_selected_movie_id(CharsPointerCopyWrapper newMovieId)
    {
        selected_movie_id = newMovieId;
        check_selected_movie_id();
    }

    void check_selected_movie_id()
    {
        const char *selected_movie_id_chars = selected_movie_id.get_chars();

        if (strlen(selected_movie_id_chars) == 0)
        {
            _selected_movie_index = 0;
            sync_movie_id_from_index();
            return;
        }

        _selected_movie_index = -1;
        Movie *current_movie = nullptr;

        do
        {
            _selected_movie_index++;
            current_movie = get_selected_movie();
        } while (
            current_movie != nullptr &&
            strcmp(current_movie->id.get_chars(), selected_movie_id_chars) != 0);
    }

    void inc_selected_movie_id(int delta)
    {
        _selected_movie_index += delta;
        sync_movie_id_from_index();
    }

    int get_movie_index_last()
    {
        return movies.size() - 1;
    }

    Movie *get_selected_movie()
    {
        int movie_index_last = get_movie_index_last();

        if (_selected_movie_index < 0 || _selected_movie_index > movie_index_last)
        {
            return nullptr;
        }

        return movies.at(_selected_movie_index);
    }

    void sync_movie_id_from_index()
    {
        int movies_size = movies.size();

        if (!movies_size)
        {
            return;
        }

        int movie_index_last = movies_size - 1;

        if (_selected_movie_index < 0)
        {
            _selected_movie_index = movie_index_last;
        }

        if (_selected_movie_index > movie_index_last)
        {
            _selected_movie_index = 0;
        }

        Movie *selected_movie = movies.at(_selected_movie_index);

        if (selected_movie == nullptr)
        {
            _selected_movie_index = 0;
            return sync_movie_id_from_index();
        }

        selected_movie_id = selected_movie->id;
    }
};

#endif // SAVEGAME_H
