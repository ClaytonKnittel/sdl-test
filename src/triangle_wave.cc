#include "src/triangle_wave.h"

#include <cmath>

#include <SDL2/SDL_audio.h>

#include "absl/time/time.h"

#include "src/note.h"

namespace sdl {

TriangleWave::TriangleWave(float hz, absl::Duration duration)
    : TimedNote(duration), hz_(hz) {}

float TriangleWave::NextNote(const SDL_AudioSpec& audio_spec,
                             const TimedNoteData& data) {
  float position =
      fmodf(hz_ * data.produced_samples_ / static_cast<float>(audio_spec.freq) +
                0.25F,
            1.F);
  if (position < 0.5) {
    return 2 * position - 1;
    // NOLINTNEXTLINE
  } else {
    return 2 * (1 - position);
  }
}

}  // namespace sdl
