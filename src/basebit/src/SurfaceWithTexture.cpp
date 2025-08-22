#include "SurfaceWithTexture.h"

#include "Renderer.h"
#include "error.h"

#include "basebit/Color.h"

#include <meadow/array.h>

#include <absl/cleanup/cleanup.h>

namespace basebit
{
namespace
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
} // namespace

SurfaceWithTexture::SurfaceWithTexture(Renderer& renderer, int w, int h)
    : width(w)
    , height(h)
    , dirty_area(SDL_Rect{0, 0, w, h})
{
    surface.reset(TRY_SDL_FN(SDL_CreateSurface, w, h, SDL_PIXELFORMAT_RGBA128_FLOAT));
    assert(!SDL_MUSTLOCK(surface.get()));
    clear();
    texture_handle = create_texture(renderer, w, h).first;
}

void SurfaceWithTexture::clear()
{
    TRY_SDL_FN(SDL_ClearSurface, surface.get(), 0.0, 0.0, 0.0, 0.0);
}

void SurfaceWithTexture::invalidate_all()
{
    dirty_area = SDL_Rect{0, 0, width, height};
}

void SurfaceWithTexture::render(Renderer& renderer)
{
    SDL_Texture* sdl_texture = renderer.get_texture_or_nullptr(texture_handle);
    if (!sdl_texture) {
        std::tie(texture_handle, sdl_texture) = create_texture(renderer, width, height);
        dirty_area = SDL_Rect{0, 0, width, height};
    }

    // dirty_area = SDL_Rect{0, 0, width, height};

    if (!SDL_RectEmpty(&dirty_area)) {
        SDL_Surface* surface_for_texture; // Freed internally when unlock.
        TRY_SDL_FN(SDL_LockTextureToSurface, sdl_texture, &dirty_area, &surface_for_texture);
        auto cleanup_lock_texture = absl::Cleanup([&] {
            SDL_UnlockTexture(sdl_texture);
        });

        SDL_Rect dst_rect{0, 0, dirty_area.w, dirty_area.h};
        TRY_SDL_FN(SDL_BlitSurfaceUnchecked, surface.get(), &dirty_area, surface_for_texture, &dst_rect);
    }

    TRY_SDL_FN(SDL_RenderTexture, renderer.get(), sdl_texture, nullptr, nullptr);

    dirty_area = SDL_Rect{0, 0, 0, 0};
}

void SurfaceWithTexture::plot(int x, int y, const Color& c)
{
    if (x < 0 || width <= x || y < 0 || height <= y) {
        return;
    }
    auto& pixel = reinterpret_cast<AF4*>(reinterpret_cast<byte*>(surface->pixels) + y * surface->pitch)[x];
    pixel = c.get_srgb_linear_f32();
    SDL_Rect rect_of_point{x, y, 1, 1};
    TRY_SDL_FN(SDL_GetRectUnion, &dirty_area, &rect_of_point, &dirty_area);
}

} // namespace basebit
