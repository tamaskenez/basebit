#pragma once

#include "basebit/export.h"

namespace basebit
{
struct BASEBIT_EXPORT Resolution {
    static const Resolution commodore_64;
    static const Resolution zx_spectrum;

    int width = 0, height = 0;
    int border_width = 0, border_height = 0;
    int char_grid_width = 0, char_grid_height = 0;
    int char_width = 0, char_height = 0;

    Resolution() = default;
    Resolution(int w, int h, int bw, int bh, int cw, int ch);
    int full_width() const
    {
        return width + 2 * border_width;
    }
    int full_height() const
    {
        return height + 2 * border_height;
    }
};
} // namespace basebit
