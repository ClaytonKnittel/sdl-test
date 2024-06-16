#include "src/sdl/renderer.h"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>

#include "absl/strings/str_cat.h"

#include "src/sdl/window.h"

namespace sdl {

Renderer::Renderer(Renderer&& renderer) noexcept
    : renderer_(renderer.renderer_) {
  renderer.renderer_ = nullptr;
}

Renderer::~Renderer() {
  if (renderer_ != nullptr) {
    SDL_DestroyRenderer(renderer_);
  }
}

// static
absl::StatusOr<Renderer> Renderer::CreateRenderer(Window& window, int index,
                                                  Uint32 flags) {
  SDL_Renderer* renderer = SDL_CreateRenderer(window.SdlWindow(), index, flags);
  if (renderer == nullptr) {
    return absl::InternalError(
        absl::StrCat("Failed to create SDL renderer: ", SDL_GetError()));
  }

  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);

  return Renderer(renderer);
}

SDL_Renderer* Renderer::SdlRenderer() {
  return renderer_;
}

Renderer::Renderer(SDL_Renderer* renderer) : renderer_(renderer) {}

}  // namespace sdl
