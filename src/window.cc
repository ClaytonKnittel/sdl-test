#include "src/window.h"

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_stdinc.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_cat.h"

#include "src/sdl_utils.h"

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

void Window::SetBackgroundColor(SDL_Color color) {
  SDL_Surface* surface = SDL_GetWindowSurface(window_);
  SDL_FillRect(surface, NULL, ColorToRGB(surface->format, color));
  SDL_UpdateWindowSurface(window_);
}

SDL_Window* Window::SdlWindow() {
  return window_;
}

Window::Window(SDL_Window* window) : window_(window) {}

}  // namespace sdl
