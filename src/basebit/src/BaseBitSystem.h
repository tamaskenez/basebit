#pragma once

#include "basebit/basebit.h"

#include "sdl_util/sdl_unique_ptr.h"

#include <meadow/array.h>
#include <meadow/cppext.h>

namespace basebit
{

struct Surface {
    Surface() = default;
    Surface(Resolution& resolution_arg, int pixel_size_arg)
        : resolution(resolution_arg)
        , pixel_size(pixel_size_arg)
        , pixels(iicast<size_t>(resolution.full_width() * resolution.full_height()))
    {
    }
    Resolution resolution;
    int pixel_size = 1;
    vector<RGB> pixels;
};

class BaseBitSystem
{
public:
    vector<SDL_DisplayID> displays;
    struct Main {
        sdl_unique_ptr<SDL_Window> window;
        sdl_unique_ptr<SDL_Renderer> renderer;
        sdl_unique_ptr<SDL_Texture> texture;
    } main;
    bool interactive = false;

    explicit BaseBitSystem(vector<SDL_DisplayID> displaysArg);
    ~BaseBitSystem();

    void set_interactive(bool enable);
    void create_window(const char* title, float height_to_screen_ratio, const Resolution& resolution);

    void set_background_color(RGB c);
    void set_border_color(RGB c);
    void clear_window();

    void exec();

private:
    void interactive_update();
    void update_texture_from_surface();
    void update_screen();

    Surface surface;
    RGB background_color, border_color;
};
} // namespace basebit
