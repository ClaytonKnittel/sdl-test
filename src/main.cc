#include "SDL2/SDL.h"
#include "SDL2/SDL_error.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_pixels.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_surface.h"
#include "SDL2/SDL_video.h"
#include "absl/log/globals.h"
#include "absl/log/initialize.h"
#include "absl/log/log.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_cat.h"

#include "src/utils.h"

absl::StatusOr<SDL_Window*> CreateWindow() {
  SDL_Window* window =
      SDL_CreateWindow("test window", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 1000, 800, SDL_WINDOW_SHOWN);

  if (window == nullptr) {
    return absl::InternalError(
        absl::StrCat("Failed to create SDL window: ", SDL_GetError()));
  }

  return window;
}

absl::StatusOr<SDL_Renderer*> CreateRenderer(SDL_Window* window) {
  SDL_Renderer* renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr) {
    return absl::InternalError(
        absl::StrCat("Failed to create SDL renderer: ", SDL_GetError()));
  }

  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);

  int img_flags = IMG_INIT_PNG;
  if (!(IMG_Init(img_flags) & img_flags)) {
    return absl::InternalError(
        absl::StrCat("Failed to initialize PNG loading: ", SDL_GetError()));
  }

  return renderer;
}

absl::StatusOr<SDL_Texture*> LoadTexture(SDL_Renderer* renderer,
                                         const std::string& path) {
  // Load image at specified path
  SDL_Surface* loadedSurface = IMG_Load(path.c_str());
  if (loadedSurface == NULL) {
    return absl::InternalError(
        absl::StrCat("Unable to load image ", path, ": ", IMG_GetError()));
  }

  // Create texture from surface pixels
  SDL_Texture* newTexture =
      SDL_CreateTextureFromSurface(renderer, loadedSurface);
  if (newTexture == NULL) {
    return absl::InternalError(absl::StrCat("Unable to create texture from ",
                                            path, ": ", SDL_GetError()));
  }

  SDL_FreeSurface(loadedSurface);
  return newTexture;
}

absl::Status Run() {
  DEFINE_OR_RETURN(SDL_Window*, window, CreateWindow());
  DEFINE_OR_RETURN(SDL_Renderer*, renderer, CreateRenderer(window));
  DEFINE_OR_RETURN(
      SDL_Texture*, texture,
      LoadTexture(renderer, "res/Madeline_Idle_Animation_(No_Backpack).png"));

  SDL_Surface* surface = SDL_GetWindowSurface(window);
  SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 100, 240, 246));
  SDL_UpdateWindowSurface(window);

  bool loop = true;
  while (loop) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT: {
          loop = false;
          break;
        }
        case SDL_KEYDOWN: {
          LOG(INFO) << "Pressed " << (char) event.key.keysym.sym;
          break;
        }
        case SDL_KEYUP: {
          LOG(INFO) << "Released " << (char) event.key.keysym.sym;
          break;
        }
      }
    }

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  // TODO: wrap these guys in objects so these can be called by destructor.
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  return absl::OkStatus();
}

int main(int argc, char* argv[]) {
  absl::InitializeLog();
  absl::SetStderrThreshold(absl::LogSeverity::kInfo);

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    LOG(ERROR) << "Failed to initialize SDL window: " << SDL_GetError();
    return -1;
  }

  auto status = Run();
  SDL_Quit();

  if (!status.ok()) {
    LOG(ERROR) << status.message();
  }
  return status.raw_code();
}
