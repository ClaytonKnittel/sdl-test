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

#include "src/renderer.h"
#include "src/texture.h"
#include "src/utils.h"
#include "src/window.h"

absl::Status Run() {
  DEFINE_OR_RETURN(sdl::Window, window,
                   sdl::Window::CreateWindow(
                       "test window", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 1000, 800, SDL_WINDOW_SHOWN));
  LOG(INFO) << "Got window";

  DEFINE_OR_RETURN(
      sdl::Renderer, renderer,
      sdl::Renderer::CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED));
  RETURN_IF_ERROR(renderer.InititalizeImage(IMG_INIT_PNG));

  DEFINE_OR_RETURN(
      sdl::Texture, texture,
      sdl::Texture::LoadFromImage(
          renderer, "res/Madeline_Idle_Animation_(No_Backpack).png"));

  SDL_Surface* surface = SDL_GetWindowSurface(window.SdlWindow());
  SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 100, 240, 246));
  SDL_UpdateWindowSurface(window.SdlWindow());

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
          if (event.key.keysym.sym == 'q') {
            loop = false;
          }
          break;
        }
        case SDL_KEYUP: {
          LOG(INFO) << "Released " << (char) event.key.keysym.sym;
          break;
        }
      }
    }

    SDL_RenderClear(renderer.SdlRenderer());
    SDL_RenderCopy(renderer.SdlRenderer(), texture.SdlTexture(), NULL, NULL);
    SDL_RenderPresent(renderer.SdlRenderer());
  }

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
