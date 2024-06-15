#include "src/note.h"

#include <SDL2/SDL_audio.h>

#include "absl/time/time.h"

#include "src/audio_buffer.h"

namespace sdl {

Note::Note(absl::Duration duration, const SDL_AudioSpec& audio_spec)
    : sample_lifetime_(static_cast<uint64_t>(absl::ToDoubleSeconds(duration) *
                                             audio_spec.freq)),
      produced_samples_(0),
      frequency_(audio_spec.freq) {}

bool Note::PopulateBufferWithNext(AudioBuffer& buffer) {
  for (auto it = buffer.begin();
       it != buffer.end() && produced_samples_ < sample_lifetime_; ++it) {
    it.WriteAll(GenerateNextNote());
    produced_samples_++;
  }
  return produced_samples_ == sample_lifetime_;
}

}  // namespace sdl
