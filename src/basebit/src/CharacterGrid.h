#pragma once

#include "sdl_util/sdl_unique_ptr.h"

#include <meadow/cppext.h>

namespace basebit
{

constexpr int k_null_charset_handle = 0;

class Renderer;
struct CharsetInSurface;

struct CharacterGrid {
    int width = 0, height = 0;
    struct Cell {
        int charset_handle = k_null_charset_handle;
        int char_code;
    };
    vector<Cell> grid;

    CharacterGrid(int w, int h)
        : width(w)
        , height(h)
        , grid(iicast<size_t>((w + 2) * (h + 2)))
    {
    }
};

// The character grid covers the area inside the border and a 1-character thick additional margin around it.
struct CharacterGridWithTexture {
    CharacterGrid grid;
    int char_width, char_height;
    int texture_handle;
    SDL_Rect dirty_area;

    CharacterGridWithTexture(Renderer& renderer, int w, int h, int char_w, int char_h);

    void clear(); // Clear to {k_null_charset_handle, 0}
    void print(int x, int y, int charset_code, int char_code);
    void render(Renderer& renderer, const unordered_map<int, CharsetInSurface>& charsets);
};
} // namespace basebit
