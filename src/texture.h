#pragma once

#include <string>

#include "absl/status/statusor.h"

#include "src/renderer.h"

struct SDL_Texture;

namespace sdl {

class Texture {
 public:
  Texture(Texture&&) noexcept;
  ~Texture();

  Texture& operator=(Texture&&) = delete;

  static absl::StatusOr<Texture> LoadFromImage(Renderer& renderer,
                                               const std::string& path);

  SDL_Texture* SdlTexture();

 private:
  explicit Texture(SDL_Texture* texture);

  SDL_Texture* texture_;
};

}  // namespace sdl
