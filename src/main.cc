#include "SDL2/SDL.h"
#include "SDL2/SDL_error.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_pixels.h"
#include "SDL2/SDL_surface.h"
#include "SDL2/SDL_video.h"
#include "absl/log/globals.h"
#include "absl/log/initialize.h"
#include "absl/log/log.h"

int Run() {
  SDL_Window* window =
      SDL_CreateWindow("test window", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 500, 300, SDL_WINDOW_SHOWN);

  if (window == nullptr) {
    LOG(ERROR) << "Failed to create SDL window: " << SDL_GetError();
    return -1;
  }

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
  }

  return 0;
}

int main(int argc, char* argv[]) {
  absl::InitializeLog();
  absl::SetStderrThreshold(absl::LogSeverity::kInfo);

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    LOG(ERROR) << "Failed to initialize SDL window: " << SDL_GetError();
    return -1;
  }

  int res = Run();
  SDL_Quit();
  return res;
}
