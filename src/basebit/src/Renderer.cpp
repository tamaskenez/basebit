#include "Renderer.h"

#include "error.h"

#include <atomic>

namespace basebit
{
namespace
{
std::atomic<int> s_next_texture_handle = 1;
}
int Renderer::add_texture(sdl_unique_ptr<SDL_Texture> texture)
{
    if (!renderer.get()) {
        throw std::logic_error("Renderer::add_texture: renderer is nullptr");
    }
    SDL_Renderer* textures_renderer = TRY_SDL_FN(SDL_GetRendererFromTexture, texture.get());
    if (textures_renderer != renderer.get()) {
        throw std::logic_error("Renderer::add_texture: texture was created with a different SDL_Renderer.");
    }
    auto itb = textures.emplace(s_next_texture_handle++, MOVE(texture));
    if (!itb.second) {
        throw std::logic_error("Renderer::add_texture: s_next_texture_handle was already in use.");
    }
    return itb.first->first;
}

SDL_Texture* Renderer::get_texture_or_nullptr(int handle) const
{
    auto it = textures.find(handle);
    return it == textures.end() ? nullptr : it->second.get();
}
} // namespace basebit
