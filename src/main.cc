#include <algorithm>
#include <iterator>
#include <memory>
#include <mutex>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>

#include "absl/log/globals.h"
#include "absl/log/initialize.h"
#include "absl/log/log.h"
#include "absl/status/status.h"
#include "absl/time/clock.h"
#include "absl/time/time.h"

#include "src/busy_calculator.h"
#include "src/framerate_throttle.h"
#include "src/sdl/rect.h"
#include "src/sdl/renderer.h"
#include "src/sdl/sound/audio_buffer.h"
#include "src/sdl/sound/audio_device.h"
#include "src/sdl/sound/note.h"
#include "src/sdl/sound/sin_wave.h"
#include "src/sdl/sound/triangle_wave.h"
#include "src/sdl/sound/volume.h"
#include "src/sdl/window.h"
#include "src/utils.h"

class AudioState {
 public:
  AudioState() : busy_calc_(absl::Now()) {}

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

  void BeginFrame() {
    lock_.lock();
    busy_calc_.BeginFrame(absl::Now());
    lock_.unlock();
  }

  void EndFrame() {
    lock_.lock();
    busy_calc_.EndFrame(absl::Now());
    lock_.unlock();
  }

 private:
  std::vector<std::unique_ptr<sdl::Note>> waves_;
  std::vector<std::unique_ptr<sdl::Note>> new_notes_;
  game::BusyCalculator busy_calc_;
  std::mutex lock_;
};

void DoAudio(void* udata, Uint8* stream, int stream_len,
             const SDL_AudioSpec& audio_spec) {
  auto* state = static_cast<AudioState*>(udata);
  state->BeginFrame();

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
  state->EndFrame();
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

  // DEFINE_OR_RETURN(
  //     sdl::Texture, texture,
  //     sdl::Texture::LoadFromImage(
  //         renderer, "res/Madeline_Idle_Animation_(No_Backpack).png"));

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

  sdl::Renderer::EntityId id1 =
      renderer.AddDrawable(std::make_unique<sdl::shape::Rect>(
                               SDL_FRect{
                                   .x = 100,
                                   .y = 100,
                                   .w = 800,
                                   .h = 600,
                               },
                               SDL_Color{
                                   .r = 0xff,
                                   .g = 0x50,
                                   .b = 0xa0,
                                   .a = 0xff,
                               }),
                           sdl::DrawPriority::kForeground);
  sdl::Renderer::EntityId id2 =
      renderer.AddDrawable(std::make_unique<sdl::shape::Rect>(
                               SDL_FRect{
                                   .x = 50,
                                   .y = 50,
                                   .w = 500,
                                   .h = 300,
                               },
                               SDL_Color{
                                   .r = 0x40,
                                   .g = 0x80,
                                   .b = 0x90,
                                   .a = 0xff,
                               }),
                           sdl::DrawPriority::kBackground);

  // for (auto freq : { 1., 6. / 5, 9. / 5, 11. / 5 }) {
  //   std::unique_ptr<sdl::TimedNote> note =
  //       std::make_unique<sdl::SinWave>(330. * freq, absl::Seconds(2));
  //   audio_state.AddNewNote(
  //       std::make_unique<sdl::Volume>(0.035, std::move(note)));
  // }
  // for (auto freq : { 1., 8. / 7, 10. / 7, 12. / 7 }) {
  //   std::unique_ptr<sdl::TimedNote> note =
  //       std::make_unique<sdl::SinWave>(330. * freq, absl::Seconds(2));
  //   audio_state.AddNewNote(
  //       std::make_unique<sdl::Volume>(0.035, std::move(note)));
  // }
  // for (auto freq : { 1., 7. / 6, 9. / 6, 11. / 6 }) {
  //   std::unique_ptr<sdl::TimedNote> note =
  //       std::make_unique<sdl::SinWave>(330. * freq, absl::Seconds(2));
  //   audio_state.AddNewNote(
  //       std::make_unique<sdl::Volume>(0.035, std::move(note)));
  // }
  // for (auto freq : { 1., 9. / 8, 12. / 8, 15. / 8 }) {
  //   std::unique_ptr<sdl::TimedNote> note =
  //       std::make_unique<sdl::SinWave>(330. * freq, absl::Seconds(2));
  //   audio_state.AddNewNote(
  //       std::make_unique<sdl::Volume>(0.035, std::move(note)));
  // }
  // for (auto freq : { 1., 11. / 9, 13. / 9, 17. / 9 }) {
  //   std::unique_ptr<sdl::TimedNote> note =
  //       std::make_unique<sdl::SinWave>(330. * freq, absl::Seconds(2));
  //   audio_state.AddNewNote(
  //       std::make_unique<sdl::Volume>(0.035, std::move(note)));
  // }

  bool loop = true;
  bool shift_held = false;
  bool ctrl_held = false;
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
          if (event.key.keysym.sym == SDLK_LSHIFT) {
            shift_held = true;
          }
          if (event.key.keysym.sym == SDLK_CAPSLOCK) {
            ctrl_held = true;
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
              freq = -1;
              break;
          }
          if (freq > 0) {
            std::unique_ptr<sdl::TimedNote> note;
            if (ctrl_held) {
              freq *= 0.94387;
            }
            if (shift_held) {
              note = std::make_unique<sdl::TriangleWave>(freq / 2,
                                                         absl::Seconds(2));
            } else {
              note = std::make_unique<sdl::SinWave>(freq, absl::Seconds(2));
            }
            audio_state.AddNewNote(
                std::make_unique<sdl::Volume>(0.035, std::move(note)));
          }
          break;
        }
        case SDL_KEYUP: {
          if (event.key.keysym.sym == SDLK_LSHIFT) {
            shift_held = false;
          }
          if (event.key.keysym.sym == SDLK_CAPSLOCK) {
            ctrl_held = false;
          }
          break;
        }
        default:
          break;
      }
    }
    if (!loop) {
      break;
    }

    sdl::shape::Rect* r1 =
        static_cast<sdl::shape::Rect*>(renderer.FindDrawable(id1).value());
    sdl::shape::Rect* r2 =
        static_cast<sdl::shape::Rect*>(renderer.FindDrawable(id2).value());

    r1->SetRect({
        .x = r1->InnerRect().x + 1,
        .y = r1->InnerRect().y,
        .w = r1->InnerRect().w,
        .h = r1->InnerRect().h,
    });
    r2->SetColor({
        .r = static_cast<Uint8>(r2->Color().r + 1),
        .g = r2->Color().g,
        .b = r2->Color().b,
        .a = r2->Color().a,
    });

    renderer.Render();

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
