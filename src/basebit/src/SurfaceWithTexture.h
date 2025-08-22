#pragma once

#include "sdl_util/sdl_unique_ptr.h"

#include <SDL3/SDL_rect.h>

namespace basebit
{

class Renderer;

// To manage the bitmap layer of the display content.
struct SurfaceWithTexture {
    int width, height;
    sdl_unique_ptr<SDL_Surface> surface;
    int texture_handle;

    SurfaceWithTexture(Renderer& renderer, int w, int h);

    void clear(); // Clear surface to translucent black.

    void update_texture(Renderer& renderer);

private:
    SDL_Rect dirty_area; // The part of the texture that should be updated.
};
} // namespace basebit
