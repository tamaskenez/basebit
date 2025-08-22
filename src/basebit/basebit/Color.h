#pragma once

#include "basebit/export.h"

#include <array>
#include <string_view>

namespace basebit
{
class BASEBIT_EXPORT Color
{
public:
    Color();
    Color(int r, int g, int b, int a = 255); // sRGB values 0-255.
    Color(std::string_view rgb_hex);         // sRGB hex string with an optional alpha (6 or 8 digits)
    std::array<uint8_t, 4> get_srgb_u8() const;
    std::array<float, 4> get_srgb_f32() const;
    const std::array<float, 4>& get_srgb_linear_f32() const
    {
        return rgba;
    }

private:
    std::array<float, 4> rgba; // Linear sRGB.
};
} // namespace basebit
