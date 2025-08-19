#include "error.h"

#include "basebit/basebit.h"

#include <SDL3/SDL_error.h>

#include <format>

namespace basebit
{
void throw_sdl_error(std::string_view sdl_function_name)
{
    throw Error(std::format("SDL function `{}` failed: {}", sdl_function_name, SDL_GetError()));
}
} // namespace basebit
