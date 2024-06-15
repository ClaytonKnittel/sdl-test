#include "src/sin_wave.h"

#include <SDL2/SDL_audio.h>

#include "absl/time/time.h"

#include "src/note.h"

namespace sdl {

constexpr float kPi = 3.14159265359F;

SinWave::SinWave(float hz, absl::Duration duration)
    : TimedNote(duration), hz_(hz) {}

float SinWave::NextNote(const SDL_AudioSpec& audio_spec,
                        const TimedNoteData& data) {
  float sample =
      sinf32((hz_ * data.produced_samples_) / audio_spec.freq * (2 * kPi));
  return sample;
}

}  // namespace sdl
