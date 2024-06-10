#include "src/window.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_stdinc.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_cat.h"

namespace sdl {

Window::Window(Window&& window) : window_(window.window_) {
  window.window_ = nullptr;
}

Window::~Window() {
  if (window_ != nullptr) {
    SDL_DestroyWindow(window_);
  }
}

// static
absl::StatusOr<Window> Window::CreateWindow(const std::string& title, int x,
                                            int y, int w, int h, Uint32 flags) {
  SDL_Window* window = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);

  if (window == nullptr) {
    return absl::InternalError(
        absl::StrCat("Failed to create SDL window: ", SDL_GetError()));
  }

  return Window(window);
}

SDL_Window* Window::SdlWindow() {
  return window_;
}

Window::Window(SDL_Window* window) : window_(window) {}

}  // namespace sdl
