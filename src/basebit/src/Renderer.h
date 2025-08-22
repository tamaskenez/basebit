#pragma once

#include "sdl_util/sdl_unique_ptr.h"

#include <meadow/cppext.h>

namespace basebit
{
class Renderer
{
    sdl_unique_ptr<SDL_Renderer> renderer;
    unordered_map<int, sdl_unique_ptr<SDL_Texture>> textures;

public:
    explicit Renderer(sdl_unique_ptr<SDL_Renderer> rendererArg)
        : renderer(MOVE(rendererArg))
    {
        assert(renderer.get());
    }
    SDL_Renderer* get() const
    {
        return renderer.get();
    }
    int add_texture(sdl_unique_ptr<SDL_Texture> texture); // Return handle.
    SDL_Texture* get_texture_or_nullptr(int handle) const;
};
} // namespace basebit
