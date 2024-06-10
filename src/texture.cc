#include "src/texture.h"

#include "SDL2/SDL_image.h"
#include "SDL2/SDL_render.h"
#include "absl/base/nullability.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_cat.h"

#include "src/renderer.h"

namespace sdl {

Texture::Texture(Texture&& texture) : texture_(texture.texture_) {
  texture.texture_ = nullptr;
}

Texture::~Texture() {
  if (texture_ != nullptr) {
    SDL_DestroyTexture(texture_);
  }
}

// static
absl::StatusOr<Texture> Texture::LoadFromImage(Renderer& renderer,
                                               const std::string& path) {
  // Load image at specified path
  SDL_Surface* loadedSurface = IMG_Load(path.c_str());
  if (loadedSurface == NULL) {
    return absl::InternalError(
        absl::StrCat("Unable to load image ", path, ": ", IMG_GetError()));
  }

  // Create texture from surface pixels
  SDL_Texture* newTexture =
      SDL_CreateTextureFromSurface(renderer.SdlRenderer(), loadedSurface);
  if (newTexture == NULL) {
    return absl::InternalError(absl::StrCat("Unable to create texture from ",
                                            path, ": ", SDL_GetError()));
  }

  SDL_FreeSurface(loadedSurface);
  return Texture(newTexture);
}

SDL_Texture* Texture::SdlTexture() {
  return texture_;
}

Texture::Texture(SDL_Texture* texture) : texture_(texture) {}

}  // namespace sdl
