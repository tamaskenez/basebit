#include <gtest/gtest.h>

#include "basebit/Color.h"

#include <meadow/cppext.h>

namespace bb = basebit;

namespace
{

float srgb_to_linear(float c)
{
    if (c <= .04045f) {
        return c / 12.92f;
    } else {
        return pow((c + 0.055f) / 1.055f, 2.4f);
    }
}

} // namespace

TEST(Color, T1)
{
    for (size_t component : {0u, 1u, 2u, 3u, 4u}) {
        for (int value : {0, 128, 255}) {
            array<int, 4> srgb;
            array<uint8_t, 4> expected_srgb_u8;
            array<float, 4> expected_srgb_f32;
            array<float, 4> expected_srgb_linear_f32;
            for (size_t c : vi::iota(0u, 4u)) {
                auto v = (c == component || component == 4u) ? value : 0;
                srgb[c] = v;
                expected_srgb_u8[c] = iicast<uint8_t>(v);
                expected_srgb_f32[c] = srgb[c] / 255.0f;
                expected_srgb_linear_f32[c] = c == 3 ? expected_srgb_f32[c] : srgb_to_linear(expected_srgb_f32[c]);
            }
            const auto color = bb::Color(srgb[0], srgb[1], srgb[2], srgb[3]);
            const auto actual_srgb_u8 = color.get_srgb_u8();
            const auto actual_srgb_f32 = color.get_srgb_f32();
            const auto actual_srgb_linear_f32 = color.get_srgb_linear_f32();

            EXPECT_EQ(actual_srgb_u8, expected_srgb_u8);

            for (size_t c : vi::iota(0u, 4u)) {
                EXPECT_NEAR(actual_srgb_f32[c], expected_srgb_f32[c], 1e-6);
                EXPECT_NEAR(actual_srgb_linear_f32[c], expected_srgb_linear_f32[c], 1e-6);
            }
        }
    }
}
