#include "ContentWindow.h"

#include "error.h"

#include "basebit/Color.h"

namespace basebit
{
void ContentWindow::clear()
{
    bitmap_layer.clear();
    if (char_grid) {
        char_grid->clear();
    }
}

void ContentWindow::update_window_from_content(UNUSED const Color& border_color, const Color& background_color)
{
    // Layer#1: Clear to background color.
    auto c = background_color.get_srgb_f32();
    TRY_SDL_FN(
      SDL_SetRenderDrawColorFloat, renderer.get(), c[0], c[1], c[2], c[3]
    ); // TODO: check if setting renderer colorspace to srgb_linear makes this different.
    TRY_SDL_FN(SDL_RenderClear, renderer.get());
#if 0
    // Layer#2: Draw bitmap.
    // Layer#3: Draw character grid.
    update_texture_from_surface();
    if (!SDL_RenderTexture(renderer.get(), main.texture.get(), nullptr, nullptr)) {
        throw_sdl_error("SDL_RenderTexture");
    }
#endif
#if 1
    // Layer#4: Draw border.
    c = border_color.get_srgb_f32();
    TRY_SDL_FN(
      SDL_SetRenderDrawColorFloat, renderer.get(), c[0], c[1], c[2], c[3]
    ); // TODO: check if setting renderer colorspace to srgb_linear makes this different.
    array<SDL_FRect, 4> rects;
    const float left_border_right = resolution.border_width;
    const float right_border_left = resolution.border_width + resolution.width;
    const float full_width = resolution.full_width();
    const float top_border_bottom = resolution.border_height;
    const float bottom_border_top = resolution.border_height + resolution.height;
    rects[0] = SDL_FRect{0, 0, full_width, top_border_bottom};
    rects[1] = SDL_FRect{0, top_border_bottom, left_border_right, bottom_border_top - top_border_bottom};
    rects[2] =
      SDL_FRect{right_border_left, top_border_bottom, left_border_right, bottom_border_top - top_border_bottom};
    rects[3] = SDL_FRect{0, bottom_border_top, full_width, top_border_bottom};
    TRY_SDL_FN(SDL_RenderFillRects, renderer.get(), rects.data(), rects.size());
#endif
    TRY_SDL_FN(SDL_RenderPresent, renderer.get());
}
} // namespace basebit
