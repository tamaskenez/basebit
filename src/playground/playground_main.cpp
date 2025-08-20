#include "basebit/basebit.h"

#include "sdl_util/sdlcpp.h"

#include <meadow/cppext.h>

#include <SDL3/SDL.h>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <thread>

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[])
{
    basebit::init();
    basebit::set_interactive(true);
    basebit::create_window("Everything is fine.", 0.6f, basebit::Resolution(320, 200, 32, 36));
    float m = 255.0f;
    basebit::set_border_color(basebit::RGB{113 / m, 109 / m, 235 / m});
    basebit::set_background_color(basebit::RGB{46 / m, 44 / m, 155 / m});
    basebit::clear_window();

    int num_renderers = SDL_GetNumRenderDrivers();
    for (int i = 0; i < num_renderers; ++i) {
        println("RenderDriver: {}", SDL_GetRenderDriver(i));
    }

    auto displays = sdl::GetDisplays().value();
    size_t count = 0;
    for (auto id : displays) {
        println("Display#{}: {}", count++, SDL_GetDisplayName(id));
        auto* dm = SDL_GetDesktopDisplayMode(id);
        println("\tformat: {}", SDL_GetPixelFormatName(dm->format));
        println("\tsize: {} x {}", dm->w, dm->h);
        println("\tpixel density: {}", dm->pixel_density);
        println(
          "\trefresh rate: {} ({}/{})", dm->refresh_rate, dm->refresh_rate_numerator, dm->refresh_rate_denominator
        );
        SDL_Rect rect;
        SDL_GetDisplayBounds(id, &rect);
        println("\tbounds: ({}, {}) ({} x {})", rect.x, rect.y, rect.w, rect.h);
        SDL_GetDisplayUsableBounds(id, &rect);
        println("\tusable bounds: ({}, {}) ({} x {})", rect.x, rect.y, rect.w, rect.h);
        println("\tcontent scale: {}", SDL_GetDisplayContentScale(id));
    }
    return EXIT_SUCCESS;
}
