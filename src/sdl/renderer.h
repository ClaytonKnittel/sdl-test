#pragma once

#include <SDL2/SDL_stdinc.h>

#include "absl/status/statusor.h"

#include "src/sdl/window.h"

struct SDL_Renderer;

namespace sdl {

class Renderer {
 public:
  Renderer(Renderer&&) noexcept;
  ~Renderer();

  Renderer& operator=(Renderer&&) = delete;

  static absl::StatusOr<Renderer> CreateRenderer(Window& window, int index,
                                                 Uint32 flags);

  SDL_Renderer* SdlRenderer();

 private:
  explicit Renderer(SDL_Renderer* renderer);

  SDL_Renderer* renderer_;
};

}  // namespace sdl
