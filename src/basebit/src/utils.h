#pragma once

#include <utility>

struct SDL_Texture;

namespace basebit
{
class Renderer;
std::pair<int, SDL_Texture*> create_texture(Renderer& renderer, int w, int h);
} // namespace basebit
