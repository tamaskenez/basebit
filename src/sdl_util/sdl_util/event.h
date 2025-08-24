#pragma once

#include <SDL3/SDL_events.h>

#include <string>

namespace sdl
{
std::string event_type_to_name(SDL_EventType event_type);
}
