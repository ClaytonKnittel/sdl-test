#include "src/texture.h"

#include "SDL2/SDL_image.h"
#include "SDL2/SDL_render.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_cat.h"

#include "src/renderer.h"

namespace sdl {

Texture::Texture(Texture&& texture) noexcept : texture_(texture.texture_) {
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
  SDL_Surface* loaded_surface = IMG_Load(path.c_str());
  if (loaded_surface == nullptr) {
    return absl::InternalError(
        absl::StrCat("Unable to load image ", path, ": ", IMG_GetError()));
  }

  // Create texture from surface pixels
  SDL_Texture* new_texture =
      SDL_CreateTextureFromSurface(renderer.SdlRenderer(), loaded_surface);
  if (new_texture == nullptr) {
    return absl::InternalError(absl::StrCat("Unable to create texture from ",
                                            path, ": ", SDL_GetError()));
  }

  SDL_FreeSurface(loaded_surface);
  return Texture(new_texture);
}

SDL_Texture* Texture::SdlTexture() {
  return texture_;
}

Texture::Texture(SDL_Texture* texture) : texture_(texture) {}

}  // namespace sdl
