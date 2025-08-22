#include "CharacterGrid.h"

namespace basebit
{
void CharacterGridWithTexture::clear()
{
    ra::fill(grid.grid, 0u);
}

} // namespace basebit
