#pragma once

#include "ContentWindow.h"

#include "basebit/basebit.h"

#include "sdl_util/sdl_unique_ptr.h"

#include <meadow/array.h>
#include <meadow/cppext.h>

namespace basebit
{
class BaseBitSystem
{
    optional<ContentWindow> content_window;
    Color border_color_, background_color_;

public:
    bool interactive = false;

    BaseBitSystem();
    ~BaseBitSystem();

    void set_interactive(bool enable);
    void create_window(const char* title, float height_to_screen_ratio, const Resolution& resolution);

    void border_color(const Color& c);
    void background_color(const Color& c);
    void clear();

    void exec();

private:
    void interactive_update();
};
} // namespace basebit
