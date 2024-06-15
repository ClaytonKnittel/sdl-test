#include <math.h>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>

#include "absl/log/globals.h"
#include "absl/log/initialize.h"
#include "absl/log/log.h"
#include "absl/status/status.h"
#include "absl/time/clock.h"
#include "absl/time/time.h"

#include "src/audio_buffer.h"
#include "src/audio_device.h"
#include "src/framerate_throttle.h"
#include "src/renderer.h"
#include "src/sin_wave.h"
#include "src/texture.h"
#include "src/utils.h"
#include "src/window.h"

struct AudioState {
  std::vector<sdl::SinWave> sin_waves;
};

void DoAudio(void* udata, Uint8* stream, int stream_len,
             const SDL_AudioSpec& audio_spec) {
  auto* state = static_cast<AudioState*>(udata);

  Uint8 channels = audio_spec.channels;

  sdl::AudioBuffer audio_buffer(stream, stream_len, channels);

  if (state->sin_waves.empty()) {
    for (int i = 0; i < 4; i++) {
      state->sin_waves.emplace_back(220.F * (i + 2), absl::Seconds(2),
                                    audio_spec);
    }
  }
  for (auto& wave : state->sin_waves) {
    wave.PopulateBufferWithNext(audio_buffer);
  }

  auto res = audio_buffer.TranslateBuffer(audio_spec.format);
  if (!res.ok()) {
    LOG(ERROR) << res;
    SDL_memset(stream, 0, stream_len);
  }
}

absl::Status Run() {
  DEFINE_OR_RETURN(sdl::Window, window,
                   sdl::Window::CreateWindow(
                       "test window", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 1000, 800, SDL_WINDOW_SHOWN));
  RETURN_IF_ERROR(window.InititalizeImage(IMG_INIT_PNG));

  DEFINE_OR_RETURN(
      sdl::Renderer, renderer,
      sdl::Renderer::CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED));

  DEFINE_OR_RETURN(
      sdl::Texture, texture,
      sdl::Texture::LoadFromImage(
          renderer, "res/Madeline_Idle_Animation_(No_Backpack).png"));

  AudioState audio_state;
  sdl::AudioDeviceBuilder audio_builder;
  DEFINE_OR_RETURN(sdl::AudioDevice, audio_device,
                   audio_builder.WithFrequency(44100)
                       .WithChannels(2)
                       .WithSamples(4096)
                       .WithFormat(AUDIO_F32LSB)
                       .WithCallback(DoAudio)
                       .WithUserdata(&audio_state)
                       .Build());

  window.SetBackgroundColor(SDL_Color{
      .r = 100,
      .g = 240,
      .b = 246,
  });

  game::FramerateThrottle throttle(/*target_fps=*/60, absl::Now());

  bool loop = true;
  while (loop) {
    throttle.BeginFrame(absl::Now());

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT: {
          loop = false;
          break;
        }
        case SDL_KEYDOWN: {
          LOG(INFO) << "Pressed " << static_cast<char>(event.key.keysym.sym);
          if (event.key.keysym.sym == 'q') {
            loop = false;
          }
          break;
        }
        case SDL_KEYUP: {
          LOG(INFO) << "Released " << static_cast<char>(event.key.keysym.sym);
          break;
        }
        default:
          break;
      }
    }

    SDL_RenderClear(renderer.SdlRenderer());
    SDL_RenderCopy(renderer.SdlRenderer(), texture.SdlTexture(), nullptr,
                   nullptr);
    SDL_RenderPresent(renderer.SdlRenderer());

    absl::Time now = absl::Now();
    throttle.EndFrame(now);
  }

  return absl::OkStatus();
}

int main(int /*argc*/, char* /*argv*/[]) {
  absl::InitializeLog();
  absl::SetStderrThreshold(absl::LogSeverity::kInfo);

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
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
