#pragma once

#include "basebit/export.h"

#include "basebit/Color.h"
#include "basebit/Error.h"
#include "basebit/Resolution.h"

#include <array>
#include <stdexcept>
#include <string>

namespace basebit
{
using RGB = std::array<float, 3>;

BASEBIT_EXPORT void init();
BASEBIT_EXPORT void set_interactive(bool enable);
BASEBIT_EXPORT void create_window(const char* title, float height_to_screen_ratio, const Resolution& resolution);
BASEBIT_EXPORT void set_background_color(RGB c);
BASEBIT_EXPORT void set_border_color(RGB c);
BASEBIT_EXPORT void clear_window();
BASEBIT_EXPORT void exec();
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
