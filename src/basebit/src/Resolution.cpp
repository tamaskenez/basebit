#include "basebit/Resolution.h"

#include "basebit/Error.h"

#include <meadow/cppext.h>

namespace basebit
{
// const Resolution Resolution::commodore_64 = Resolution(320, 200, 32, 36);
const Resolution Resolution::commodore_64 = Resolution(320, 200, (384 - 320) / 2, (288 - 200) / 2, 40, 25);
const Resolution Resolution::zx_spectrum = Resolution(256, 192, (384 - 256) / 2, (288 - 192) / 2, 32, 24);

Resolution::Resolution(int w, int h, int bw, int bh, int cw, int ch)
    : width(w)
    , height(h)
    , border_width(bw)
    , border_height(bh)
    , char_grid_width(cw)
    , char_grid_height(ch)
    , char_width(width / char_grid_width)
    , char_height(height / char_grid_height)
{
    if (width % char_grid_width != 0 || height % char_grid_height != 0) {
        throw Error(format(
          "The bitmap size ({} x {}) must be divisible by the character grid size ({} x {})",
          width,
          height,
          char_grid_width,
          char_grid_height
        ));
    }
}
} // namespace basebit
