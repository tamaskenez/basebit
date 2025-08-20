#include "basebit/basebit.h"

#include "BaseBitSystem.h"
#include "error.h"
#include "global.h"

#include <meadow/cppext.h>

#include "sdl_util/sdl_unique_ptr.h"
#include "sdl_util/sdlcpp.h"

#include <SDL3/SDL_init.h>

namespace basebit
{
void init()
{
    g_basebit.reset();
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        throw_sdl_error("SDL_Init");
    }

    auto displays = sdl::GetDisplays();
    if (!displays) {
        throw Error(displays.error());
    }
    // Note, we might get zero displays, which is not an error.

    g_basebit = make_unique<BaseBitSystem>(MOVE(*displays));
}

void set_interactive(bool enable)
{
    g_basebit->set_interactive(enable);
}

void create_window(const char* title, float height_to_screen_ratio, const Resolution& resolution)
{
    g_basebit->create_window(title, height_to_screen_ratio, resolution);
}

void set_background_color(RGB c)
{
    g_basebit->set_background_color(c);
}

void set_border_color(RGB c)
{
    g_basebit->set_border_color(c);
}

void clear_window()
{
    g_basebit->clear_window();
}

void exec()
{
    g_basebit->exec();
}

} // namespace basebit
