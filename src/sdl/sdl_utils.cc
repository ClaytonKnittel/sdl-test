#include "src/sdl/sdl_utils.h"

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>

namespace sdl {

Uint32 ColorToRGB(const SDL_PixelFormat* format, SDL_Color color) {
  return SDL_MapRGB(format, color.r, color.g, color.b);
}

Uint32 ColorToRGBA(const SDL_PixelFormat* format, SDL_Color color) {
  return SDL_MapRGBA(format, color.r, color.g, color.b, color.a);
}

}  // namespace sdl
