#include "basebit/Color.h"

#include "basebit/Error.h"

#include "error.h"

#include <meadow/cppext.h>

#include <SDL3/SDL_surface.h>

#include <charconv>

namespace basebit
{

namespace
{
void ConvertPixelsAndColorspace_rgba888_srgb_to_rgba128_float_srgb_linear(
  const array<uint8_t, 4>& src, array<float, 4>& dst
)
{
    if (!SDL_ConvertPixelsAndColorspace(
          1,
          1,
          SDL_PIXELFORMAT_RGBA32,
          SDL_COLORSPACE_SRGB,
          0,
          src.data(),
          1,
          SDL_PIXELFORMAT_RGBA128_FLOAT,
          SDL_COLORSPACE_SRGB_LINEAR,
          0,
          dst.data(),
          1
        )) {
        throw_sdl_error("SDL_ConvertPixelsAndColorspace");
    }
}

void ConvertPixelsAndColorspace_rgba128_float_srgb_linear_to_rgba888_srgb(
  const array<float, 4>& src, array<uint8_t, 4>& dst
)
{
    if (!SDL_ConvertPixelsAndColorspace(
          1,
          1,
          SDL_PIXELFORMAT_RGBA128_FLOAT,
          SDL_COLORSPACE_SRGB_LINEAR,
          0,
          src.data(),
          1,
          SDL_PIXELFORMAT_RGBA32,
          SDL_COLORSPACE_SRGB,
          0,
          dst.data(),
          1
        )) {
        throw_sdl_error("SDL_ConvertPixelsAndColorspace");
    }
}

void ConvertPixelsAndColorspace_rgba128_float_srgb_linear_to_rgba128_float_srgb(
  const array<float, 4>& src, array<float, 4>& dst
)
{
    if (!SDL_ConvertPixelsAndColorspace(
          1,
          1,
          SDL_PIXELFORMAT_RGBA128_FLOAT,
          SDL_COLORSPACE_SRGB_LINEAR,
          0,
          src.data(),
          1,
          SDL_PIXELFORMAT_RGBA128_FLOAT,
          SDL_COLORSPACE_SRGB,
          0,
          dst.data(),
          1
        )) {
        throw_sdl_error("SDL_ConvertPixelsAndColorspace");
    }
}
} // namespace

Color::Color()
{
    rgba.fill(0.0f);
}

Color::Color(int r, int g, int b, int a)
{
    if (r < 0 || 255 < r || g < 0 || 255 < g || b < 0 || 255 < b || a < 0 || 255 < a) {
        throw Error("All components must be between 0 and 255");
    }
    array<uint8_t, 4> src = {iicast<uint8_t>(r), iicast<uint8_t>(g), iicast<uint8_t>(b), iicast<uint8_t>(a)};
    ConvertPixelsAndColorspace_rgba888_srgb_to_rgba128_float_srgb_linear(src, rgba);
}

Color::Color(string_view rgb_hex)
{
    if (!rgb_hex.empty() && rgb_hex[0] == '#') {
        rgb_hex.remove_prefix(1);
    }
    if (rgb_hex.size() != 6 || rgb_hex.size() != 8) {
        throw Error("Hex color code must be 6 or 8 hex digits, optionally prefixed by '#'");
    }
    UNUSED size_t ix = 0;
    array<uint8_t, 4> src;
    for (; !rgb_hex.empty(); ++ix) {
        auto d0 = rgb_hex[0];
        auto d1 = rgb_hex[1];
        rgb_hex.remove_prefix(2);
        if (!isxdigit(d0) || !isxdigit(d1)) {
            throw Error("Hex color code must contain hex digits: 0-9, a-f, A-F");
        }
        auto fcr = std::from_chars(rgb_hex.data(), rgb_hex.data() + 2, src[ix], 16);
        if (fcr.ec != std::errc()) {
            throw Error(format(
              "Can't convert hex number: \"{}\", reason: {}",
              rgb_hex.substr(0, 2),
              std::error_condition(fcr.ec).message()
            ));
        }
    }
    if (ix < 4) {
        src[3] = 255;
    }
    ConvertPixelsAndColorspace_rgba888_srgb_to_rgba128_float_srgb_linear(src, rgba);
}

array<uint8_t, 4> Color::get_srgb_u8() const
{
    array<uint8_t, 4> dst;
    ConvertPixelsAndColorspace_rgba128_float_srgb_linear_to_rgba888_srgb(rgba, dst);
    return dst;
}

array<float, 4> Color::get_srgb_f32() const
{
    array<float, 4> dst;
    ConvertPixelsAndColorspace_rgba128_float_srgb_linear_to_rgba128_float_srgb(rgba, dst);
    return dst;
}
} // namespace basebit
