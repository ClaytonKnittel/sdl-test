#include "src/note.h"

#include <optional>

#include <SDL2/SDL_audio.h>

#include "absl/time/time.h"

#include "src/audio_buffer.h"

namespace sdl {

std::optional<float> Note::GenerateNextNote() {
  if (done_) {
    return std::nullopt;
  }

  auto next = GenerateNextNoteImpl();
  if (!next.has_value()) {
    done_ = true;
  }
  return next;
}

bool Note::PopulateBuffer(AudioBuffer& buffer) {
  std::optional<float> next_note;
  for (auto it = buffer.begin();
       it != buffer.end() && (next_note = GenerateNextNote()).has_value();
       ++it) {
    it.WriteAll(next_note.value());
  }
  return next_note.has_value();
}

TimedNote::TimedNote(absl::Duration duration, const SDL_AudioSpec& audio_spec)
    : sample_lifetime_(static_cast<uint64_t>(absl::ToDoubleSeconds(duration) *
                                             audio_spec.freq)),
      produced_samples_(0),
      frequency_(audio_spec.freq) {}

std::optional<float> TimedNote::GenerateNextNoteImpl() {
  if (produced_samples_ == sample_lifetime_) {
    return std::nullopt;
  }
  float res = NextNote();
  produced_samples_++;
  return res;
}

}  // namespace sdl
