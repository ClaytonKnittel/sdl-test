#pragma once

#include <cstdint>

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

namespace sdl {

class Drawable {
 public:
  Drawable() = default;
  virtual ~Drawable() = default;

  // Returns the number of vertices that the Drawable needs currently. Must
  // always stay the same.
  //
  // TODO: If necessary, allow Size() to change and reallocate space in
  // drawbales_.
  virtual uint64_t Size() const = 0;

  // Renders `Size()` vertices to the given vertex array.
  virtual void Render(SDL_Vertex* vertices) = 0;
};

}  // namespace sdl
