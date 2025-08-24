#include "CharsetInSurface.h"

#include "basebit/Charset.h"
#include "error.h"

#include <meadow/cppext.h>

namespace basebit
{
CharsetInSurface::CharsetInSurface(const Charset& cs, SDL_Palette* palette)
    : width(cs.width())
    , height(cs.height())
    , surface(TRY_SDL_FN(SDL_CreateSurface, width, height * iicast<int>(cs.size()), SDL_PIXELFORMAT_INDEX8))
    , code_to_surface_y(detail::write_charset_into_sdl_surface(cs, surface.get()))
{
    assert(!SDL_MUSTLOCK(surface.get()));
    TRY_SDL_FN(SDL_SetSurfacePalette, surface.get(), palette);
}
} // namespace basebit
