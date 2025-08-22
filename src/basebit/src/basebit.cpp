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

// https://www.pepto.de/projects/colorvic/
const std::vector<Color> Palette::commodore_64 = {
  Color(0, 0, 0),       // #0 black
  Color(255, 255, 255), // #1 white
  Color(129, 51, 56),   // #2 red
  Color(116, 206, 200), // #3 cyan
  Color(142, 60, 152),  // #4 purple
  Color(86, 172, 77),   // #5 green
  Color(46, 44, 155),   // #6 blue
  Color(237, 241, 112), // #7 yellow
  Color(142, 80, 41),   // #8 orange
  Color(85, 56, 0),     // #9 brown
  Color(196, 108, 113), // #10 light red
  Color(74, 74, 74),    // #11 dark grey
  Color(123, 123, 123), // #12 grey
  Color(169, 255, 159), // #13 light green
  Color(113, 109, 235), // #14 light blue
  Color(178, 178, 178)  // #15 light grey
};

// https://en.wikipedia.org/wiki/ZX_Spectrum_graphic_modes
const std::vector<Color> Palette::zx_spectrum = {
  Color("#000000"), // #0 black
  Color("#0100ce"), // #1 blue
  Color("#cf0100"), // #2 red
  Color("#cf01ce"), // #3 magenta
  Color("#00cf15"), // #4 green
  Color("#01cfcf"), // #5 cyan
  Color("#cfcf15"), // #6 yellow
  Color("#cfcfcf"), // #7 white
  Color("#000000"), // #8 bright black
  Color("#0200fd"), // #9 bright blue
  Color("#ff0201"), // #10 bright red
  Color("#ff02fd"), // #11 bright magenta
  Color("#00ff1c"), // #12 bright green
  Color("#02ffff"), // #13 bright cyan
  Color("#ffff1d"), // #14 bright yellow
  Color("#ffffff")  // #15 bright white
};

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

void border_color(const Color& c)
{
    g_basebit->border_color(c);
}

void border_color(int palette_ix)
{
    g_basebit->border_color(palette_ix);
}

void background_color(const Color& c)
{
    g_basebit->background_color(c);
}

void background_color(int palette_ix)
{
    g_basebit->background_color(palette_ix);
}

void color(const Color& c)
{
    g_basebit->color(c);
}

void color(int palette_ix)
{
    g_basebit->color(palette_ix);
}

void clear()
{
    g_basebit->clear();
}

void exec()
{
    g_basebit->exec();
}

void palette(std::vector<Color> colors)
{
    g_basebit->palette(MOVE(colors));
}

void plot(int x, int y)
{
    g_basebit->plot(x, y);
}

} // namespace basebit
