#include <algorithm>
#include <iterator>
#include <memory>
#include <mutex>
#include <optional>
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
#include "src/note.h"
#include "src/renderer.h"
#include "src/sin_wave.h"
#include "src/texture.h"
#include "src/utils.h"
#include "src/volume.h"
#include "src/window.h"

class AudioState {
 public:
  const std::vector<std::unique_ptr<sdl::Note>>& Waves() {
    return waves_;
  }

  void UpdateState() {
    waves_.erase(std::remove_if(waves_.begin(), waves_.end(),
                                [](const std::unique_ptr<sdl::Note>& note) {
                                  return note->Done();
                                }),
                 waves_.end());

    lock_.lock();
    waves_.insert(waves_.end(), std::make_move_iterator(new_notes_.begin()),
                  std::make_move_iterator(new_notes_.end()));
    new_notes_.clear();
    lock_.unlock();
  }

  void AddNewNote(std::unique_ptr<sdl::Note> note) {
    lock_.lock();
    new_notes_.push_back(std::move(note));
    lock_.unlock();
  }

 private:
  std::vector<std::unique_ptr<sdl::Note>> waves_;
  std::vector<std::unique_ptr<sdl::Note>> new_notes_;
  std::mutex lock_;
};

void DoAudio(void* udata, Uint8* stream, int stream_len,
             const SDL_AudioSpec& audio_spec) {
  auto* state = static_cast<AudioState*>(udata);

  Uint8 channels = audio_spec.channels;

  sdl::AudioBuffer audio_buffer(stream, stream_len, channels);

  state->UpdateState();
  for (const auto& wave : state->Waves()) {
    wave->PopulateBuffer(audio_buffer, audio_spec);
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
                       .WithSamples(1024)
                       .WithFormat(AUDIO_F32LSB)
                       .WithCallback(DoAudio)
                       .WithUserdata(&audio_state)
                       .Build());
  (void) audio_device;

  window.SetBackgroundColor(SDL_Color{
      .r = 100,
      .g = 240,
      .b = 246,
  });

  game::FramerateThrottle throttle(/*target_fps=*/60, absl::Now());

  bool loop = true;
  while (true) {
    throttle.BeginFrame(absl::Now());

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT: {
          loop = false;
          break;
        }
        case SDL_KEYDOWN: {
          if (event.key.keysym.sym == 'q') {
            loop = false;
          }

          float freq;
          switch (event.key.keysym.sym) {
            case 'a': {
              freq = 440;
              break;
            }
            case 'b': {
              freq = 493.883;
              break;
            }
            case 'c': {
              freq = 523.251;
              break;
            }
            case 'd': {
              freq = 587.330;
              break;
            }
            case 'e': {
              freq = 659.255;
              break;
            }
            case 'f': {
              freq = 698.456;
              break;
            }
            case 'g': {
              freq = 783.991;
              break;
            }
            default:
              break;
          }
          audio_state.AddNewNote(std::make_unique<sdl::Volume>(
              0.035, std::make_unique<sdl::SinWave>(freq, absl::Seconds(2))));
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
    if (!loop) {
      break;
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
