#pragma once

#include "CharacterGrid.h"
#include "Renderer.h"
#include "SurfaceWithTexture.h"

#include "basebit/Resolution.h"

#include "sdl_util/sdl_unique_ptr.h"

namespace basebit
{
class Color;
struct ContentWindow {
    Resolution resolution;
    int pixel_size = 1;
    sdl_unique_ptr<SDL_Window> window; // The single, global window.
    Renderer renderer;                 // Renderer for the window.
    SurfaceWithTexture bitmap_layer;
    optional<CharacterGridWithTexture> char_grid;

    void clear(); // Clear bitmap to translucent black, char_grid to char#0.
    void update_window_from_content(const Color& border_color, const Color& background_color);
};
} // namespace basebit
