#include "src/sdl/renderer.h"

#include <cstddef>
#include <cstdint>
#include <memory>

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>

#include "absl/status/status.h"
#include "absl/strings/str_cat.h"

#include "src/sdl/drawable.h"
#include "src/sdl/window.h"

namespace sdl {

using EntityId = Renderer::EntityId;

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

void Renderer::Render() {
  SDL_RenderClear(renderer_);
  for (auto& vertices : vertices_) {
    SDL_memset(vertices.data(), 0, vertices.size() * sizeof(SDL_Vertex));
  }

  for (const auto& [id, entry] : drawables_) {
    entry.drawable->Render(
        vertices_[static_cast<size_t>(entry.priority)].data() +
        entry.first_vertex_idx);
  }

  for (auto& vertices : vertices_) {
    SDL_RenderGeometry(renderer_, nullptr, vertices.data(), vertices.size(),
                       nullptr, 0);
  }
  SDL_RenderPresent(renderer_);
}

EntityId Renderer::AddDrawable(std::unique_ptr<Drawable> drawable,
                               DrawPriority priority) {
  EntityId id = next_id_++;

  const uint64_t size = drawable->Size();
  const auto array_idx = static_cast<size_t>(priority);
  const uint64_t first_vertex_idx = vertices_[array_idx].size();
  vertices_[array_idx].resize(first_vertex_idx + size);

  drawables_.insert({
      id,
      DrawableEntry{
          .drawable = std::move(drawable),
          .priority = priority,
          .size = size,
          .first_vertex_idx = first_vertex_idx,
      },
  });
  return id;
}

absl::Status Renderer::RemoveDrawable(EntityId id) {
  auto it = drawables_.find(id);
  if (it == drawables_.end()) {
    return absl::InternalError(absl::StrCat("No such entity with id ", id));
  }

  // TODO: track unused memory, reallocate/shrink vector later.
  drawables_.erase(it);
  return absl::OkStatus();
}

Renderer::Renderer(SDL_Renderer* renderer)
    : renderer_(renderer), next_id_(1000) {}

}  // namespace sdl
