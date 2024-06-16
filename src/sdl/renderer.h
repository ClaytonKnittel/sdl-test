#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"

#include "src/sdl/drawable.h"
#include "src/sdl/window.h"

struct SDL_Renderer;

namespace sdl {

class Renderer {
 public:
  using EntityId = uint64_t;

  Renderer(Renderer&&) noexcept;
  ~Renderer();

  Renderer& operator=(Renderer&&) = delete;

  static absl::StatusOr<Renderer> CreateRenderer(Window& window, int index,
                                                 Uint32 flags);

  SDL_Renderer* SdlRenderer();

  void Render();

  EntityId AddDrawable(std::unique_ptr<Drawable> drawable);

  absl::Status RemoveDrawable(EntityId id);

 private:
  struct DrawableEntry {
    std::unique_ptr<Drawable> drawable;
    uint64_t size;
    uint64_t first_vertex_idx;
  };

  explicit Renderer(SDL_Renderer* renderer);

  SDL_Renderer* renderer_;

  absl::flat_hash_map<EntityId, DrawableEntry> drawables_;
  std::vector<SDL_Vertex> vertices_;

  EntityId next_id_;
};

}  // namespace sdl
