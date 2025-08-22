#pragma once

#include "basebit/export.h"

#include "basebit/Color.h"
#include "basebit/Error.h"
#include "basebit/Resolution.h"

#include <array>
#include <stdexcept>
#include <string>
#include <vector>

namespace basebit
{
namespace Palette
{
extern const std::vector<Color> commodore_64;
extern const std::vector<Color> zx_spectrum;
} // namespace Palette

BASEBIT_EXPORT void init();
BASEBIT_EXPORT void set_interactive(bool enable);
BASEBIT_EXPORT void create_window(const char* title, float height_to_screen_ratio, const Resolution& resolution);
BASEBIT_EXPORT void background_color(const Color& c);
BASEBIT_EXPORT void background_color(int palette_ix);
BASEBIT_EXPORT void border_color(const Color& c);
BASEBIT_EXPORT void border_color(int palette_ix);
BASEBIT_EXPORT void clear();
BASEBIT_EXPORT void exec();
BASEBIT_EXPORT void palette(std::vector<Color> colors);
} // namespace basebit

/*
 #include <span>
 #include <string_view>
using RGBA=std::array<float,4>;
class CharacterSet;
void bb_set_fullscreen(bool enabled);
void bb_set_palette(std::span<RGB> palette);
void bb_set_background_color(int palette_ix);
void bb_set_color(int palette_ix);
void bb_set_color(RGB c);
void bb_set_border_color(int palette_ix);
void bb_set_character_set(const CharacterSet&cs);
void bb_print(std::string_view s);
*/
