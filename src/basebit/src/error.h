#pragma once

#include <string_view>

namespace basebit
{
[[noreturn]] void throw_sdl_error(std::string_view sdl_function_name);
}
