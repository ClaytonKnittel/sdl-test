#include "src/sdl/sound/note.h"

#include <optional>

#include <SDL2/SDL_audio.h>

#include "absl/time/time.h"

#include "src/sdl/sound/audio_buffer.h"

namespace sdl {

std::optional<float> Note::GenerateNextNote(const SDL_AudioSpec& audio_spec) {
  if (done_) {
    return std::nullopt;
  }

  auto next = GenerateNextNoteImpl(audio_spec);
  if (!next.has_value()) {
    done_ = true;
  }
  return next;
}

bool Note::PopulateBuffer(AudioBuffer& buffer,
                          const SDL_AudioSpec& audio_spec) {
  std::optional<float> next_note;
  for (auto it = buffer.begin();
       it != buffer.end() &&
       (next_note = GenerateNextNote(audio_spec)).has_value();
       ++it) {
    it.WriteAll(next_note.value());
  }
  return next_note.has_value();
}

TimedNote::TimedNote(absl::Duration duration) : duration_(duration) {}

std::optional<float> TimedNote::GenerateNextNoteImpl(
    const SDL_AudioSpec& audio_spec) {
  if (!data_.has_value()) {
    data_ = TimedNoteData{
      .sample_lifetime_ = static_cast<uint64_t>(
          absl::ToDoubleSeconds(duration_) * audio_spec.freq),
      .produced_samples_ = 0,
    };
  }

  TimedNoteData& data = data_.value();
  if (data.produced_samples_ == data.sample_lifetime_) {
    return std::nullopt;
  }

  float res = NextNote(audio_spec, data);
  data.produced_samples_++;
  return res;
}

}  // namespace sdl
