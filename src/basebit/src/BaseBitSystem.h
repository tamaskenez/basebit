#pragma once

#include "CharsetInSurface.h"
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
    Color border_color_, background_color_, color_;
    vector<Color> palette_;
    sdl_unique_ptr<SDL_Palette> char_palette;
    unordered_map<int, CharsetInSurface> charsets;
    int current_charset_handle = k_null_charset_handle;

public:
    bool interactive = false;

    BaseBitSystem();
    ~BaseBitSystem();

    void set_interactive(bool enable);
    void create_window(const char* title, float height_to_screen_ratio, const Resolution& resolution);

    void background_color(const Color& c);
    void background_color(int palette_ix);
    void border_color(const Color& c);
    void border_color(int palette_ix);
    void color(const Color& c);
    void color(int palette_ix);

    void clear();

    void palette(std::vector<Color> colors);
    void plot(int x, int y);

    int add_charset(const Charset& cs);
    void charset(int charset_handle);
    void print(int x, int y, std::string_view chars);
    void print(int x, int y, int code);

    void exec();

private:
    void interactive_update();
};
} // namespace basebit
