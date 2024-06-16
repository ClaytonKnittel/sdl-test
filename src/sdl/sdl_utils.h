#pragma once

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>

namespace sdl {

// Converts an SDL_Color to a Uint32 RGB representation.
Uint32 ColorToRGB(const SDL_PixelFormat* format, SDL_Color color);

// Converts an SDL_Color to a Uint32 RGBA representation.
Uint32 ColorToRGBA(const SDL_PixelFormat* format, SDL_Color color);

}  // namespace sdl
