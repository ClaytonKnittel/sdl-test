#include "src/sin_wave.h"

#include <SDL2/SDL_audio.h>

#include "absl/time/time.h"

#include "src/note.h"

namespace sdl {

constexpr float kPi = 3.14159265359F;

SinWave::SinWave(float hz, absl::Duration duration,
                 const SDL_AudioSpec& audio_spec)
    : TimedNote(duration, audio_spec), hz_(hz) {}

float SinWave::NextNote() {
  float sample = sinf32((hz_ * produced_samples_) / frequency_ * (2 * kPi));
  return sample;
}

}  // namespace sdl
