#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
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

// Determines which layer of the canvas a drawable is drawn. The higher priority
// (i.e. the lower down the list), the farther forward the drawable is drawn
// (i.e. is drawn on top of drawables with lower priority).
enum class DrawPriority {
  // Lowest priority. Rendered in the back.
  kBackground,
  // Renders in front of background objects.
  kForeground,
  // Reserved for the UI, which renders on top of everything else.
  kUi,

  kNumDrawPriorities,
};

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

  EntityId AddDrawable(std::unique_ptr<Drawable> drawable,
                       DrawPriority priority);

  absl::Status RemoveDrawable(EntityId id);

  std::optional<Drawable*> FindDrawable(EntityId id);

 private:
  struct DrawableEntry {
    std::unique_ptr<Drawable> drawable;
    DrawPriority priority;
    uint64_t size;
    uint64_t first_vertex_idx;
  };

  explicit Renderer(SDL_Renderer* renderer);

  SDL_Renderer* renderer_;

  absl::flat_hash_map<EntityId, DrawableEntry> drawables_;
  std::vector<SDL_Vertex>
      vertices_[static_cast<size_t>(DrawPriority::kNumDrawPriorities)];

  EntityId next_id_;
};

}  // namespace sdl
