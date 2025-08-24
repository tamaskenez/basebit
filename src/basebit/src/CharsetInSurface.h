#pragma once

#include <meadow/cppext.h>

#include "sdl_util/sdl_unique_ptr.h"

namespace basebit
{
class Charset;
struct CharsetInSurface {
    int width, height;
    sdl_unique_ptr<SDL_Surface> surface;
    unordered_map<int, int> code_to_surface_y;

    CharsetInSurface(const Charset& cs, SDL_Palette* palette);
};
} // namespace basebit
