#include "basebit/basebit.h"

#include "BaseBitSystem.h"
#include "error.h"
#include "global.h"

#include <meadow/cppext.h>

#include "sdl_util/sdl_unique_ptr.h"
#include "sdl_util/sdlcpp.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_surface.h>

namespace basebit
{
void init()
{
    g_basebit.reset();
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        throw_sdl_error("SDL_Init");
    }
    g_basebit = make_unique<BaseBitSystem>();
}

void set_interactive(bool enable)
{
    g_basebit->set_interactive(enable);
}

void create_window(const char* title, float height_to_screen_ratio, const Resolution& resolution)
{
    g_basebit->create_window(title, height_to_screen_ratio, resolution);
}

void background_color(const Color& c)
{
    g_basebit->background_color(c);
}

void border_color(const Color& c)
{
    g_basebit->border_color(c);
}

void clear()
{
    g_basebit->clear();
}

void exec()
{
    g_basebit->exec();
}

} // namespace basebit
