#include "src/sdl/rect.h"

#include <cstdint>

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

namespace sdl::shape {

Rect::Rect(SDL_FRect rect, SDL_Color color) : rect_(rect), color_(color) {}

std::uint64_t Rect::Size() const {
  return 6;
}

void Rect::Render(SDL_Vertex* vertices) {
  const float lx = rect_.x;
  const float ly = rect_.y;
  const float hx = rect_.x + rect_.w;
  const float hy = rect_.y + rect_.h;

  vertices[0] = {
    .position = { .x = lx, .y = ly },
    .color = color_,
  };
  vertices[1] = {
    .position = { .x = hx, .y = ly },
    .color = color_,
  };
  vertices[2] = {
    .position = { .x = hx, .y = hy },
    .color = color_,
  };

  vertices[3] = {
    .position = { .x = lx, .y = ly },
    .color = color_,
  };
  vertices[4] = {
    .position = { .x = hx, .y = hy },
    .color = color_,
  };
  vertices[5] = {
    .position = { .x = lx, .y = hy },
    .color = color_,
  };
}

const SDL_FRect& Rect::InnerRect() const {
  return rect_;
}

void Rect::SetRect(SDL_FRect rect) {
  rect_ = rect;
}

SDL_Color Rect::Color() const {
  return color_;
}

void Rect::SetColor(SDL_Color color) {
  color_ = color;
}

}  // namespace sdl::shape
