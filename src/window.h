#pragma once

#include <string>

#include "SDL2/SDL_pixels.h"
#include "SDL2/SDL_stdinc.h"
#include "absl/status/statusor.h"

struct SDL_Window;

namespace sdl {

class Window {
 public:
  Window(Window&&) noexcept;
  ~Window();

  Window& operator=(Window&&) = delete;

  static absl::StatusOr<Window> CreateWindow(const std::string& title, int x,
                                             int y, int w, int h, Uint32 flags);

  absl::Status InititalizeImage(int flags);

  absl::Status InititalizeAudio();

  void SetBackgroundColor(SDL_Color color);

  SDL_Window* SdlWindow();

 private:
  explicit Window(SDL_Window* window);

  SDL_Window* window_;
  bool img_initialized_ = false;
  bool audio_initialized_ = false;
};

}  // namespace sdl
