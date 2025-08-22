#pragma once

#include "sdl_util/sdl_unique_ptr.h"

#include <meadow/cppext.h>

namespace basebit
{

struct CharacterGrid {
    int width = 0, height = 0;
    vector<int> grid;

    CharacterGrid(int w, int h)
        : width(w)
        , height(h)
        , grid(iicast<size_t>((w + 2) * (h + 2)))
    {
    }
};

struct CharacterGridWithTexture {
    CharacterGrid grid;
    int texture;
    sdl_unique_ptr<SDL_Surface> charSet;

    CharacterGridWithTexture(int w, int h)
        : grid(w, h)
    {
        // TODO
        assert(false);
    }

    void clear(); // Clear to char#0.
};
} // namespace basebit
