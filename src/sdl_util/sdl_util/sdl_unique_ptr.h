#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

#include <memory>

// A std::unique_ptr-like object for SDL types, like SDL_Window, SDL_Renderer, etc.. which calls the appropriate
// SDL_Destroy* function in its destructor.
template<class T>
class sdl_unique_ptr;

template<class T>
sdl_unique_ptr(T*) -> sdl_unique_ptr<T>;

#define DEFINE_SDL_UNIQUE_PTR(TYPE, DELETER_FN)                               \
    struct TYPE##_Deleter {                                                   \
        void operator()(TYPE* p) const                                        \
        {                                                                     \
            DELETER_FN(p);                                                    \
        }                                                                     \
    };                                                                        \
    template<>                                                                \
    class sdl_unique_ptr<TYPE> : public std::unique_ptr<TYPE, TYPE##_Deleter> \
    {                                                                         \
    public:                                                                   \
        sdl_unique_ptr() = default;                                           \
        explicit sdl_unique_ptr(TYPE* p)                                      \
            : std::unique_ptr<TYPE, TYPE##_Deleter>(p)                        \
        {                                                                     \
        }                                                                     \
    };

DEFINE_SDL_UNIQUE_PTR(SDL_Window, SDL_DestroyWindow)
DEFINE_SDL_UNIQUE_PTR(SDL_Renderer, SDL_DestroyRenderer)
DEFINE_SDL_UNIQUE_PTR(SDL_Texture, SDL_DestroyTexture)
DEFINE_SDL_UNIQUE_PTR(SDL_Surface, SDL_DestroySurface)

#undef DEFINE_SDL_UNIQUE_PTR
