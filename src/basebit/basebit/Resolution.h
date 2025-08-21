#pragma once

#include "basebit/export.h"

namespace basebit
{
struct BASEBIT_EXPORT Resolution {
    int width = 0, height = 0;
    int border_width = 0, border_height = 0;

    Resolution() = default;
    Resolution(int w, int h, int bw, int bh)
        : width(w)
        , height(h)
        , border_width(bw)
        , border_height(bh)
    {
    }
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
