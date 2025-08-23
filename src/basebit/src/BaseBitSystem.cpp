#include "BaseBitSystem.h"

#include "error.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_properties.h>

#include <absl/cleanup/cleanup.h>

namespace basebit
{

namespace
{
// https://wiki.libsdl.org/SDL3/SDL_CreateRendererWithProperties:
//     "direct3d11, direct3d12, and metal renderers support SDL_COLORSPACE_SRGB_LINEAR"
constexpr bool k_create_window_renderer_with_srgb_linear = false;
} // namespace

BaseBitSystem::BaseBitSystem() {}

void BaseBitSystem::set_interactive(bool enable)
{
    if (content_window) {
        TRY_SDL_FN(SDL_SetWindowAlwaysOnTop, content_window->window.get(), interactive);
    }

    bool enabled_now = !interactive && enable;
    interactive = enable;
    if (enabled_now) {
        interactive_update();
    }
}

namespace
{
struct CalculateWindowDimensionsResult {
    AI2 window_size;
    int pixel_size;
    Resolution resolution;
};

CalculateWindowDimensionsResult
calculate_window_dimensions(AI2 display_size, float height_to_screen_ratio, const Resolution& resolution)
{
    int window_height = std::min(iceil<int>(display_size[1] * height_to_screen_ratio), display_size[1]);
    int pixel_size = std::max(
      1,
      std::min(
        display_size[0] / resolution.full_width(), // max horizontal pixel size
        window_height / resolution.full_height()   // target vertical pixel size
      )
    );
    auto new_resolution = resolution;
    // Reduce resolution if doesn't fit the display.
    if (resolution.full_width() * pixel_size > display_size[0]) {
        if (resolution.width * pixel_size > display_size[0]) {
            new_resolution.width = display_size[0] / pixel_size;
            new_resolution.border_width = 0;
        } else {
            new_resolution.border_width = (display_size[0] - resolution.width * pixel_size) / pixel_size;
        }
    }
    if (resolution.full_height() * pixel_size > display_size[01]) {
        if (resolution.height * pixel_size > display_size[1]) {
            new_resolution.height = display_size[1] / pixel_size;
            new_resolution.border_height = 0;
        } else {
            new_resolution.border_height = (display_size[1] - resolution.height * pixel_size) / pixel_size;
        }
    }
    return CalculateWindowDimensionsResult{
      .window_size = {new_resolution.full_width() * pixel_size, new_resolution.full_height() * pixel_size},
      .pixel_size = pixel_size,
      .resolution = new_resolution
    };
}
} // namespace

void BaseBitSystem::create_window(const char* title, float height_to_screen_ratio, const Resolution& resolutionArg)
{
    content_window.reset();

    // Expect the window to be created on the primary display.

    auto primaryDisplayId = TRY_SDL_FN(SDL_GetPrimaryDisplay);

    auto displayId = primaryDisplayId;
    sdl_unique_ptr<SDL_Window> window;
    optional<Renderer> renderer;
    for (int pass : {0, 1}) {
        SDL_Rect rect;
        TRY_SDL_FN(SDL_GetDisplayUsableBounds, displayId, &rect);

        AI2 display_size = AI2{rect.w, rect.h};

        auto cwd_result = calculate_window_dimensions(display_size, height_to_screen_ratio, resolutionArg);

        if (pass == 0) {
            window.reset(TRY_SDL_FN(
              SDL_CreateWindow, title, cwd_result.window_size[0], cwd_result.window_size[1], SDL_WINDOW_RESIZABLE
            ));
            if ((k_create_window_renderer_with_srgb_linear)) {
                auto props_id = TRY_SDL_FN(SDL_CreateProperties);
                auto props_cleanup = absl::Cleanup([&] {
                    SDL_DestroyProperties(props_id);
                });

                TRY_SDL_FN(SDL_SetPointerProperty, props_id, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, window.get());
                TRY_SDL_FN(
                  SDL_SetNumberProperty,
                  props_id,
                  SDL_PROP_RENDERER_CREATE_OUTPUT_COLORSPACE_NUMBER,
                  SDL_COLORSPACE_SRGB_LINEAR
                );

                renderer = Renderer(sdl_unique_ptr(TRY_SDL_FN(SDL_CreateRendererWithProperties, props_id)));
            } else {
                renderer = Renderer(sdl_unique_ptr(TRY_SDL_FN(SDL_CreateRenderer, window.get(), nullptr)));
            }
        } else {
            TRY_SDL_FN(SDL_SetWindowSize, window.get(), cwd_result.window_size[0], cwd_result.window_size[1]);
        }
        if (pass == 0) {
            auto id = TRY_SDL_FN(SDL_GetDisplayForWindow, window.get());
            if (id != primaryDisplayId) {
                // Second try.
                continue;
            }
        }

        TRY_SDL_FN(SDL_SetRenderScale, renderer->get(), cwd_result.pixel_size, cwd_result.pixel_size);

        auto bitmap_layer = SurfaceWithTexture(*renderer, resolutionArg.full_width(), resolutionArg.full_height());
        content_window = ContentWindow{
          .resolution = resolutionArg,
          .pixel_size = cwd_result.pixel_size,
          .window = MOVE(window),
          .renderer = MOVE(renderer.value()),
          .bitmap_layer = MOVE(bitmap_layer)
        };
        break;
    }
    set_interactive(interactive); // For content window always on top.

    interactive_update();
}

BaseBitSystem::~BaseBitSystem()
{
    SDL_Quit();
}

void BaseBitSystem::interactive_update()
{
    if (interactive) {
#if 1
        for (int i = 0; i < 3; ++i) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {}
            if (content_window) {
                content_window->update_window_from_content(border_color_, background_color_);
            }
        }
#else
        SDL_PumpEvents();
        if (window && renderer) {
            update_screen();
            if (!SDL_RenderPresent(renderer.get())) {
                throw_sdl_error("SDL_RenderPresent");
            }
        }
#endif
    }
}

void BaseBitSystem::border_color(const Color& c)
{
    border_color_ = c;
    if (content_window) {
        interactive_update();
    }
}

void BaseBitSystem::border_color(int palette_ix)
{
    if (palette_ix < 0 || cmp_less_equal(palette_.size(), palette_ix)) {
        throw Error(std::format("Invalid palette index: {} (allowed range: 0 - {})", palette_ix, palette_.size()));
    }
    border_color(palette_[iicast<size_t>(palette_ix)]);
}

void BaseBitSystem::background_color(const Color& c)
{
    background_color_ = c;
    if (content_window) {
        interactive_update();
    }
}

void BaseBitSystem::background_color(int palette_ix)
{
    if (palette_ix < 0 || cmp_less_equal(palette_.size(), palette_ix)) {
        throw Error(std::format("Invalid palette index: {} (allowed range: 0 - {})", palette_ix, palette_.size()));
    }
    background_color(palette_[iicast<size_t>(palette_ix)]);
}

void BaseBitSystem::color(const Color& c)
{
    color_ = c;
}

void BaseBitSystem::color(int palette_ix)
{
    if (palette_ix < 0 || cmp_less_equal(palette_.size(), palette_ix)) {
        throw Error(std::format("Invalid palette index: {} (allowed range: 0 - {})", palette_ix, palette_.size()));
    }
    color(palette_[iicast<size_t>(palette_ix)]);
}

void BaseBitSystem::palette(std::vector<Color> colors)
{
    palette_ = MOVE(colors);
    if (content_window) {
        interactive_update();
    }
}

void BaseBitSystem::clear()
{
    if (content_window) {
        content_window->bitmap_layer.clear();
        if (content_window->char_grid) {
            content_window->char_grid->clear();
        }
        interactive_update();
    }
}

void BaseBitSystem::plot(int x, int y)
{
    if (!content_window) {
        throw Error("No window to plot to");
    }
    content_window->bitmap_layer.plot(
      content_window->resolution.border_width + x, content_window->resolution.border_height + y, color_
    );
    interactive_update();
}

void BaseBitSystem::exec()
{
#if 0
    auto exit_at = chr::steady_clock::now() + chr::seconds(5);
    while (chr::steady_clock::now() < exit_at) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) { // poll until all events are handled!
            // decide what to do with this event.
        }

        // update game state, draw the current frame
        update_screen();
    }
#endif
}

int BaseBitSystem::add_charset(const Charset& cs) {}

void BaseBitSystem::charset(int charset_ix) {}

} // namespace basebit
