#include "CharacterGrid.h"

#include "CharsetInSurface.h"
#include "Renderer.h"
#include "error.h"
#include "utils.h"

namespace basebit
{
CharacterGridWithTexture::CharacterGridWithTexture(Renderer& renderer, int w, int h, int char_w, int char_h)
    : grid(w, h)
    , char_width(char_w)
    , char_height(char_h)
    , texture_handle(create_texture(renderer, grid.width * char_width, grid.height * char_height).first)
    , dirty_area(SDL_Rect{0, 0, w, h})
{
    renderer.get_texture_or_nullptr(0);
}

void CharacterGridWithTexture::clear()
{
    ra::fill(grid.grid, CharacterGrid::Cell());
    dirty_area = SDL_Rect({0, 0, grid.width, grid.height});
}

void CharacterGridWithTexture::print(int x, int y, int charset_handle, int char_code)
{
    if (x < 0 || grid.width <= x || y < 0 || grid.height <= y) {
        return;
    }
    grid.grid[iicast<size_t>(x + y * grid.width)] = CharacterGrid::Cell{charset_handle, char_code};
    SDL_Rect rect_of_char{x, y, 1, 1};
    TRY_SDL_FN(SDL_GetRectUnion, &dirty_area, &rect_of_char, &dirty_area);
}

void CharacterGridWithTexture::render(
  Renderer& renderer, const unordered_map<int, CharsetInSurface>& charsets, const SDL_FRect& dst_rect
)
{
    SDL_Texture* sdl_texture = renderer.get_texture_or_nullptr(texture_handle);
    if (!sdl_texture) {
        std::tie(texture_handle, sdl_texture) =
          create_texture(renderer, grid.width * char_width, grid.height * char_height);
        dirty_area = SDL_Rect{0, 0, grid.width, grid.height};
    }

    if (!SDL_RectEmpty(&dirty_area)) {
        SDL_Surface* surface_for_texture; // Freed internally when unlock.
        SDL_Rect dirty_area_pixels{
          dirty_area.x * char_width, dirty_area.y * char_height, dirty_area.w * char_width, dirty_area.h * char_height
        };
        TRY_SDL_FN(SDL_LockTextureToSurface, sdl_texture, &dirty_area_pixels, &surface_for_texture);
        auto cleanup_lock_texture = absl::Cleanup([&] {
            SDL_UnlockTexture(sdl_texture);
        });

        TRY_SDL_FN(SDL_ClearSurface, surface_for_texture, 0, 0, 0, 0);
        for (int y : vi::iota(0, grid.height)) {
            for (int x : vi::iota(0, grid.width)) {
                auto& cell = grid.grid[iicast<size_t>(x + y * grid.width)];
                if (cell.charset_handle == k_null_charset_handle) {
                    continue;
                }
                auto charset_it = charsets.find(cell.charset_handle);
                if (charset_it == charsets.end()) {
                    continue;
                }
                auto& charset = charset_it->second;
                auto it = charset.code_to_surface_y.find(cell.char_code);
                if (it == charset.code_to_surface_y.end()) {
                    continue;
                }
                SDL_Rect src_char_rect{0, it->second, char_width, char_height};
                SDL_Rect dst_char_rect{
                  x * char_width - dirty_area_pixels.x, y * char_height - dirty_area_pixels.y, char_width, char_height
                };
                TRY_SDL_FN(
                  SDL_BlitSurfaceUnchecked, charset.surface.get(), &src_char_rect, surface_for_texture, &dst_char_rect
                );
            }
        }
    }

    TRY_SDL_FN(SDL_RenderTexture, renderer.get(), sdl_texture, nullptr, &dst_rect);

    dirty_area = SDL_Rect{0, 0, 0, 0};
}

} // namespace basebit
