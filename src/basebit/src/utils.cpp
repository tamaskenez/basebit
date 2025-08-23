#include "utils.h"

#include "Renderer.h"
#include "error.h"

#include <meadow/cppext.h>

#include <SDL3/SDL_render.h>

namespace basebit
{
pair<int, SDL_Texture*> create_texture(Renderer& renderer, int w, int h)
{
    auto texture = sdl_unique_ptr(
      TRY_SDL_FN(SDL_CreateTexture, renderer.get(), SDL_PIXELFORMAT_RGBA128_FLOAT, SDL_TEXTUREACCESS_STREAMING, w, h)
    );

#ifndef NDEBUG
    SDL_PropertiesID props_id = TRY_SDL_FN(SDL_GetTextureProperties, texture.get());
    auto cleanup_props_id = absl::Cleanup([&] {
        SDL_DestroyProperties(props_id);
    });
    assert(SDL_GetNumberProperty(props_id, SDL_PROP_TEXTURE_COLORSPACE_NUMBER, 0) == SDL_COLORSPACE_SRGB_LINEAR);
#endif

    TRY_SDL_FN(SDL_SetTextureScaleMode, texture.get(), SDL_SCALEMODE_NEAREST);

    SDL_Texture* raw_texture_ptr = texture.get();
    return pair(renderer.add_texture(MOVE(texture)), raw_texture_ptr);
}
} // namespace basebit
