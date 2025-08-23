#include "basebit/basebit.h"

#include "sdl_util/sdlcpp.h"

#include <meadow/cppext.h>

#include <SDL3/SDL.h>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <thread>

namespace bb = basebit;

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[])
{
    bb::init();
    bb::set_interactive(true);
    bb::create_window("Everything is fine.", 0.7f, bb::Resolution::commodore_64);
    bb::palette(bb::Palette::commodore_64);
    bb::border_color(14);
    bb::background_color(6);
    bb::color(1);
    if ((false)) {
        for (int x : vi::iota(0, 320)) {
            bb::plot(x, x % 20);
        }
    }
    const int c64_upper_handle = bb::add_charset(bb::Charset::commodore_64_upper);
    bb::charset(c64_upper_handle);
    bb::print(3, 5, "HELLO");
    //  basebit::exec();

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
