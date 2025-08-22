#include "error.h"

#include "basebit/basebit.h"

#include <SDL3/SDL_error.h>

#include <SDL3/SDL_platform_defines.h>

#include <cassert>
#include <format>
#include <print>

#ifdef SDL_PLATFORM_APPLE
  #include <sys/sysctl.h>
  #include <sys/types.h>
  #include <unistd.h>
#endif

#ifdef SDL_PLATFORM_WINDOWS
  #include <debugapi.h> // Check if we need something else, like Windows.h (and some WIN32_LEAN_AND_MEAN macros).
#endif

namespace basebit
{
#ifdef SDL_PLATFORM_APPLE
static bool is_debugger_attached()
{
    [[maybe_unused]] int junk;
    int mib[4];
    struct kinfo_proc info;
    size_t size;

    // Initialize the flags so that, if sysctl fails for some bizarre
    // reason, we get a predictable result.

    info.kp_proc.p_flag = 0;

    // Initialize mib, which tells sysctl the info we want, in this case
    // we're looking for information about a specific process ID.

    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_PID;
    mib[3] = getpid();

    // Call sysctl.

    size = sizeof(info);
    junk = sysctl(mib, sizeof(mib) / sizeof(*mib), &info, &size, NULL, 0);
    assert(junk == 0);

    // We're being debugged if the P_TRACED flag is set.

    return ((info.kp_proc.p_flag & P_TRACED) != 0);
}
#elifdef SDL_PLATFORM_WINDOWS
bool is_debugger_attached()
{
    return IsDebuggerPresent();
}
#else
  #warning is_debugger_attached is not implemented for this platform.
bool is_debugger_attached()
{
    return false;
}
#endif

void throw_sdl_error(std::string_view sdl_function_name)
{
    auto message = std::format("SDL function `{}` failed: {}", sdl_function_name, SDL_GetError());
    if (is_debugger_attached()) {
        println(stderr, "{}", message);
    }
    throw Error(message);
}
} // namespace basebit
