#pragma once

#include "basebit/export.h"

#include <array>
#include <stdexcept>
#include <string>

namespace basebit
{

struct BASEBIT_EXPORT Resolution {
    int width, height;
    int border_width, border_height;
    int full_width() const
    {
        return width + 2 * border_width;
    }
    int full_height() const
    {
        return height + 2 * border_height;
    }
};
using RGB = std::array<float, 3>;

BASEBIT_EXPORT void init();
BASEBIT_EXPORT void set_interactive(bool enable);
BASEBIT_EXPORT void create_window(const char* title, float height_to_screen_ratio, const Resolution& resolution);
BASEBIT_EXPORT void set_background_color(RGB c);
BASEBIT_EXPORT void set_border_color(RGB c);
BASEBIT_EXPORT void clear_window();

class Error : std::runtime_error
{
public:
    explicit Error(const std::string& msg)
        : std::runtime_error(msg)
    {
    }
    explicit Error(const char* msg)
        : std::runtime_error(msg)
    {
    }
};
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
