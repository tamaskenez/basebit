#include "CharsetInSurface.h"

namespace basebit
{
explicit    CharsetInSurface::CharsetInSurface(const Charset&cs):width(cs.width),
height(cs.height),
surface(SDL_CreateSurface(width,height*cs.size(),SDL_BITMAPORDER_1234)){
}
}
