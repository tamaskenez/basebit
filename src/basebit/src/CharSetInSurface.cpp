#include "CharsetInSurface.h"

#include "basebit/Charset.h"

#include <meadow/cppext.h>

namespace basebit
{
CharsetInSurface::CharsetInSurface(const Charset& cs)
    : width(cs.width())
    , height(cs.height())
    , surface(SDL_CreateSurface(width, height * iicast<int>(cs.size()), SDL_PIXELFORMAT_INDEX1LSB))
    , code_to_surface_y(detail::write_charset_into_sdl_surface(cs, surface.get()))
{
    assert(!SDL_MUSTLOCK(surface.get()));
}
} // namespace basebit
