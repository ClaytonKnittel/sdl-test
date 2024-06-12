#pragma once

#include <SDL2/SDL_pixels.h>
#include <string>

#include "SDL2/SDL_stdinc.h"
#include "absl/status/statusor.h"

struct SDL_Window;

namespace sdl {

class Window {
 public:
  Window(Window&&);
  ~Window();

  Window& operator=(Window&&) = delete;

  static absl::StatusOr<Window> CreateWindow(const std::string& title, int x,
                                             int y, int w, int h, Uint32 flags);

  void SetBackgroundColor(SDL_Color color);

  SDL_Window* SdlWindow();

 private:
  explicit Window(SDL_Window* window);

  SDL_Window* window_;
};

}  // namespace sdl
