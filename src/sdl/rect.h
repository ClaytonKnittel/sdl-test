#pragma once

#include <cstdint>

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

#include "src/sdl/drawable.h"

namespace sdl::shape {

class Rect : public Drawable {
 public:
  Rect(SDL_FRect rect, SDL_Color color);

  uint64_t Size() const override;

  void Render(SDL_Vertex* vertices) override;

  const SDL_FRect& InnerRect() const;

  void SetRect(SDL_FRect rect);

  SDL_Color Color() const;

  void SetColor(SDL_Color color);

 private:
  SDL_FRect rect_;
  SDL_Color color_;
};

}  // namespace sdl::shape
