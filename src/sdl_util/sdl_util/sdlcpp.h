#pragma once

#include <meadow/cppext.h>

#include <SDL3/SDL_video.h>

namespace sdl
{
expected<vector<SDL_DisplayID>, string> GetDisplays();
}
