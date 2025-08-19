#include "sdl_util/sdlcpp.h"

#include <SDL3/SDL_error.h>

namespace sdl
{
expected<vector<SDL_DisplayID>, string> GetDisplays()
{
    int num_displays;
    SDL_DisplayID* displays = SDL_GetDisplays(&num_displays);
    if (!displays) {
        return unexpected(SDL_GetError());
    }
    vector<SDL_DisplayID> result;
    result.reserve(iicast<size_t>(num_displays));
    auto* p = displays;
    for (; *p; ++p) {
        result.push_back(*p);
    }
    assert(p - displays == num_displays);
    SDL_free(displays);
    return result;
}
} // namespace sdl
