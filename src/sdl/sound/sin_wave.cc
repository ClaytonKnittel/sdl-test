#include "src/sin_wave.h"

#include <SDL2/SDL_audio.h>

#include "absl/time/time.h"

#include "src/note.h"

namespace sdl {

constexpr float kPi = 3.14159265359F;

SinWave::SinWave(float hz, absl::Duration duration)
    : TimedNote(duration), hz_2pi_(hz * 2 * kPi) {}

float SinWave::NextNote(const SDL_AudioSpec& audio_spec,
                        const TimedNoteData& data) {
  float sample = sinf32((hz_2pi_ * data.produced_samples_) / audio_spec.freq);
  return sample;
}

}  // namespace sdl
