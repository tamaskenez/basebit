#pragma once

#include "sdl_util/sdl_unique_ptr.h"

namespace basebit
{
struct CharsetInSurface
{
    int width, height;
    sdl_unique_ptr<SDL_Surface> surface;
explicit    CharsetInSurface(const Charset&cs);
};
} // namespace basebit
