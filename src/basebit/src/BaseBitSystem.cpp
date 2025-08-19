#include "BaseBitSystem.h"

#include "error.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_properties.h>

namespace basebit
{
BaseBitSystem::BaseBitSystem(vector<SDL_DisplayID> displaysArg)
    : displays(MOVE(displaysArg))
{
}

void BaseBitSystem::set_interactive(bool enable)
{
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

void BaseBitSystem::create_window(const char* title, float height_to_screen_ratio, const Resolution& resolution)
{
    main.renderer.reset();
    main.window.reset();

    // Expect the window to be created on the primary display.

    auto primaryDisplayId = SDL_GetPrimaryDisplay();
    if (primaryDisplayId == 0) {
        throw_sdl_error("SDL_GetPrimaryDisplay");
    }

    auto displayId = primaryDisplayId;
    for (int pass : {0, 1}) {
        SDL_Rect rect;
        if (!SDL_GetDisplayUsableBounds(displayId, &rect)) {
            throw_sdl_error("SDL_GetDisplayUsableBounds");
        }

        AI2 display_size = AI2{rect.w, rect.h};

        auto cwd_result = calculate_window_dimensions(display_size, height_to_screen_ratio, resolution);

        if (pass == 0) {
            main.window.reset(
              SDL_CreateWindow(title, cwd_result.window_size[0], cwd_result.window_size[1], SDL_WINDOW_RESIZABLE)
            );
            if (!main.window) {
                throw_sdl_error("SDL_CreateWindow");
            }
            main.renderer.reset(SDL_CreateRenderer(main.window.get(), nullptr));
            if (!main.renderer) {
                throw_sdl_error("SDL_CreateWindowAndRenderer");
            }
            println("Renderer name: {}", SDL_GetRendererName(main.renderer.get())); // todo remove
        } else {
            if (!SDL_SetWindowSize(main.window.get(), cwd_result.window_size[0], cwd_result.window_size[1])) {
                throw_sdl_error("SDL_SetWindowSize");
            }
        }
        if (pass == 0) {
            auto id = SDL_GetDisplayForWindow(main.window.get());
            if (id == 0) {
                throw_sdl_error("SDL_GetDisplayForWindow");
            }
            if (id != primaryDisplayId) {
                // Second try.
                continue;
            }
        }

        surface = Surface(cwd_result.resolution, cwd_result.pixel_size);
        break;
    }

    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetNumberProperty(props, SDL_PROP_TEXTURE_CREATE_COLORSPACE_NUMBER, SDL_COLORSPACE_SRGB);
    SDL_SetNumberProperty(props, SDL_PROP_TEXTURE_CREATE_FORMAT_NUMBER, SDL_PIXELFORMAT_RGB96_FLOAT);
    SDL_SetNumberProperty(props, SDL_PROP_TEXTURE_CREATE_ACCESS_NUMBER, SDL_TEXTUREACCESS_STREAMING);
    SDL_SetNumberProperty(props, SDL_PROP_TEXTURE_CREATE_WIDTH_NUMBER, surface.resolution.full_width());
    SDL_SetNumberProperty(props, SDL_PROP_TEXTURE_CREATE_HEIGHT_NUMBER, surface.resolution.full_height());

    main.texture.reset(SDL_CreateTextureWithProperties(main.renderer.get(), props));
    SDL_DestroyProperties(props);
    interactive_update();
}

BaseBitSystem::~BaseBitSystem()
{
    SDL_Quit();
}
void BaseBitSystem::update_texture_from_surface()
{
    void* dst_pixels_void;
    int dst_pitch_bytes;
    if (!SDL_LockTexture(main.texture.get(), nullptr, &dst_pixels_void, &dst_pitch_bytes)) {
        throw_sdl_error("SDL_LockTexture");
    }
    byte* dst_pixels_bytes = reinterpret_cast<byte*>(dst_pixels_void);
    const int sizeof_pixel = iicast<int>(sizeof(surface.pixels[0]));
    if (dst_pitch_bytes % sizeof_pixel != 0) {
        throw Error(format(
          "Texture pitch from SDL_LockTexture ({}) not divisible by pixel size ({})", dst_pitch_bytes, sizeof_pixel
        ));
    }
    using pixel_type = std::decay_t<decltype(surface.pixels[0])>;
    const int fw = surface.resolution.full_width();
    const int fh = surface.resolution.full_height();
    const int src_pitch_bytes = fw * sizeof_pixel;
    if (dst_pitch_bytes == src_pitch_bytes) {
        std::copy(
          surface.pixels.data(), surface.pixels.data() + fh * fw, reinterpret_cast<pixel_type*>(dst_pixels_bytes)
        );
    } else {
        for (int y = 0; y < surface.resolution.full_height(); ++y, dst_pixels_bytes += dst_pitch_bytes) {
            std::copy(
              surface.pixels.data() + y * fw,
              surface.pixels.data() + (y + 1) * fw,
              reinterpret_cast<pixel_type*>(dst_pixels_bytes)
            );
        }
    }
    SDL_UnlockTexture(main.texture.get());
}

void BaseBitSystem::interactive_update()
{
    if (interactive) {
        SDL_PumpEvents();
        if (main.window && main.renderer) {
            SDL_SetRenderDrawColor(main.renderer.get(), 0, 0, 0, 255);
            if (!SDL_RenderClear(main.renderer.get())) {
                throw_sdl_error("SDL_RenderClear");
            }
            update_texture_from_surface();
            if (!SDL_RenderTexture(main.renderer.get(), main.texture.get(), nullptr, nullptr)) {
                throw_sdl_error("SDL_RenderTexture");
            }
            if (!SDL_RenderPresent(main.renderer.get())) {
                throw_sdl_error("SDL_RenderPresent");
            }
            if (!SDL_RenderPresent(main.renderer.get())) {
                throw_sdl_error("SDL_RenderPresent");
            }
        }
    }
}

void BaseBitSystem::set_background_color(RGB c)
{
    background_color = c;
}

void BaseBitSystem::set_border_color(UNUSED RGB c)
{
    border_color = c;
}

void BaseBitSystem::clear_window()
{
    const int stride = surface.resolution.full_width();
    auto* d = surface.pixels.data();
    std::fill(d, d + surface.resolution.border_height * stride, border_color);
    const int bottom_border_y0 = surface.resolution.border_height + surface.resolution.height;
    std::fill(
      d + surface.resolution.border_height * stride,
      d + bottom_border_y0 * surface.resolution.full_width(),
      background_color
    );
    std::fill(d + bottom_border_y0 * stride, d + surface.resolution.full_height() * stride, border_color);
    const int left_border_x0 = surface.resolution.border_width + surface.resolution.width;
    for (int y = surface.resolution.border_height; y < bottom_border_y0; ++y) {
        std::fill(d + y * stride, d + y * stride + surface.resolution.border_width, border_color);
        std::fill(d + y * stride + left_border_x0, d + (y + 1) * stride, border_color);
    }
    interactive_update();
}

} // namespace basebit
