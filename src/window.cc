#include "src/window.h"

#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_pixels.h"
#include "SDL2/SDL_stdinc.h"
#include "SDL2/SDL_surface.h"
#include "SDL2/SDL_video.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_cat.h"

#include "src/sdl_utils.h"

namespace sdl {

Window::Window(Window&& window) noexcept
    : window_(window.window_),
      img_initialized_(window.img_initialized_),
      audio_initialized_(window.audio_initialized_) {
  window.window_ = nullptr;
  window.img_initialized_ = false;
  window.audio_initialized_ = false;
}

Window::~Window() {
  if (window_ != nullptr) {
    SDL_DestroyWindow(window_);
  }
  if (img_initialized_) {
    IMG_Quit();
  }
  if (audio_initialized_) {
    Mix_Quit();
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

absl::Status Window::InititalizeImage(int flags) {
  if (!(IMG_Init(flags) & flags)) {
    return absl::InternalError(
        absl::StrCat("Failed to initialize PNG loading: ", IMG_GetError()));
  }

  img_initialized_ = true;
  return absl::OkStatus();
}

absl::Status Window::InititalizeAudio() {
  if (Mix_OpenAudio(/*frequency=*/44100, MIX_DEFAULT_FORMAT, /*channels=*/2,
                    /*chunksize=*/2048) < 0) {
    return absl::InternalError(
        absl::StrCat("Failed to initialize audio: ", Mix_GetError()));
  }

  audio_initialized_ = true;
  return absl::OkStatus();
}

void Window::SetBackgroundColor(SDL_Color color) {
  SDL_Surface* surface = SDL_GetWindowSurface(window_);
  SDL_FillRect(surface, nullptr, ColorToRGB(surface->format, color));
  SDL_UpdateWindowSurface(window_);
}

SDL_Window* Window::SdlWindow() {
  return window_;
}

Window::Window(SDL_Window* window) : window_(window) {}

}  // namespace sdl
