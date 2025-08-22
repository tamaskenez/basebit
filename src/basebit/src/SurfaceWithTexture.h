#pragma once

#include "sdl_util/sdl_unique_ptr.h"

#include <SDL3/SDL_rect.h>

namespace basebit
{

class Renderer;
class Color;

// To manage the bitmap layer of the display content.
class SurfaceWithTexture
{
    int width, height;
    sdl_unique_ptr<SDL_Surface> surface;
    int texture_handle;
    SDL_Rect dirty_area; // The part of the texture that should be updated.

public:
    SurfaceWithTexture(Renderer& renderer, int w, int h);

    void clear(); // Clear surface to translucent black.
    void invalidate_all();

    void render(Renderer& renderer);

    void plot(int x, int y, const Color& c);
};
} // namespace basebit
