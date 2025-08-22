#pragma once

#include <string_view>

namespace basebit
{

// Throw `basebit::Error` with the description containing the function name and error message from `SDL_GetError()`.
[[noreturn]] void throw_sdl_error(std::string_view sdl_function_name);

template<class T>
T check_sdl_result_or_throw_error(T result, std::string_view function_name)
{
    bool success;
    if constexpr (std::is_same_v<T, bool>) {
        success = result;
    } else if constexpr (std::is_pointer_v<T>) {
        success = result != nullptr;
    } else if constexpr (std::is_integral_v<T>) {
        success = result != T(0);
    } else {
        static_assert(false, "The result type of the SDL function is expected to be bool, pointer or integral.");
    }
    if (success) {
        return result;
    }
    throw_sdl_error(function_name);
}

// Execute an (SDL) function with the specified args. If error returned, call `throw_sdl_error`.
// Error is
// - `false` for `bool` results
// - `nullptr` for pointer results
// - zero for integer results
//
#define TRY_SDL_FN(FUNCTION_NAME, ...) check_sdl_result_or_throw_error(FUNCTION_NAME(__VA_ARGS__), #FUNCTION_NAME)

} // namespace basebit
