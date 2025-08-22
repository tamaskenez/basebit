#include "basebit/Resolution.h"

namespace basebit
{
// const Resolution Resolution::commodore_64 = Resolution(320, 200, 32, 36);
const Resolution Resolution::commodore_64 = Resolution(320, 200, (384 - 320) / 2, (288 - 200) / 2);
const Resolution Resolution::zx_spectrum = Resolution(256, 192, (384 - 256) / 2, (288 - 192) / 2);
} // namespace basebit
