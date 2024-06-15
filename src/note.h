#pragma once

#include <SDL2/SDL_audio.h>

#include "absl/time/time.h"

#include "src/audio_buffer.h"

namespace sdl {

class Note {
 public:
  Note(absl::Duration duration, const SDL_AudioSpec& audio_spec);

  // Fills up the buffer with sound data. If `true` is returned, that means the
  // note was fully consumed and will not produce any more sound data.
  bool PopulateBufferWithNext(AudioBuffer& buffer);

 protected:
  // Generates the next sample for the note.
  virtual float GenerateNextNote() = 0;

  // How many samples the note will last for.
  uint64_t sample_lifetime_;

  // The total number of samples produced so far.
  uint64_t produced_samples_;

  // The frequency of the audio samples (per second).
  float frequency_;
};

}  // namespace sdl
