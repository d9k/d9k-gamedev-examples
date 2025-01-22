#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <vector>
#include "movie.h"
#include "movies.h"
#include "chars_copy.h"

class SaveGame
{
public:
    CharsPointerCopyWrapper chars_wrapper_selected_movie_id;
    int loads_count = 0;
    int _selected_movie_index = 0;

    Movies *_movies;

    SaveGame()
    {
        _movies = new Movies();
    }

    ~SaveGame()
    {
        destruct_movies();
    }

    void destruct_movies()
    {
        // int last_index = _movies->size() - 1;
        // for (int i = last_index; i >= 0; i--)
        // {
        //     delete _movies->at(i);
        // }
        delete _movies;
    }

    void set_movies(Movies *movies)
    {
        destruct_movies();
        _movies = movies;
    }

    Movies *get_movies()
    {
        return _movies;
    }

    void validate()
    {
        check_selected_movie_id();
    }

    // void set_selected_movie_id(char *newMovieId)
    // {
    //     selected_movie_id.set_chars(newMovieId);
    //     check_selected_movie_id();
    // }

    void check_selected_movie_id()
    {
        const char *chars_selected_movie_id = chars_wrapper_selected_movie_id.get_chars();

        if (strlen(chars_selected_movie_id) == 0)
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
            strcmp(current_movie->chars_wrapper_id.get_chars(), chars_selected_movie_id) != 0);
    }

    void inc_selected_movie_id(int delta)
    {
        _selected_movie_index += delta;
        sync_movie_id_from_index();
    }

    int get_movie_index_last()
    {
        return _movies->size() - 1;
    }

    Movie *get_selected_movie()
    {
        int movie_index_last = get_movie_index_last();

        if (_selected_movie_index < 0 || _selected_movie_index > movie_index_last)
        {
            return nullptr;
        }

        return &_movies->at(_selected_movie_index);
    }

    void sync_movie_id_from_index()
    {
        int movies_size = _movies->size();

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

        Movie *selected_movie = &_movies->at(_selected_movie_index);

        if (selected_movie == nullptr)
        {
            _selected_movie_index = 0;
            return sync_movie_id_from_index();
        }

        chars_wrapper_selected_movie_id.copy_fields_from(&selected_movie->chars_wrapper_id);
    }
};

#endif // SAVEGAME_H
