#pragma once

#include <SDL2/SDL_audio.h>

#include "absl/time/time.h"

#include "src/audio_buffer.h"

namespace sdl {

class Note {
 public:
  Note() = default;
  virtual ~Note() = default;

  bool Done() const {
    return done_;
  }

  // Stops the note from playing.
  void Cancel() {
    done_ = true;
  }

  // Generates the next sample for the note.
  std::optional<float> GenerateNextNote(const SDL_AudioSpec& audio_spec);

  // Fills up the buffer with sound data. If `true` is returned, that means the
  // note was fully consumed and will not produce any more sound data.
  bool PopulateBuffer(AudioBuffer& buffer, const SDL_AudioSpec& audio_spec);

 protected:
  // Returns the next sample for the note, advancing the internal state of the
  // note generator.
  virtual std::optional<float> GenerateNextNoteImpl(
      const SDL_AudioSpec& audio_spec) = 0;

 private:
  bool done_ = false;
};

class TimedNote : public Note {
 public:
  explicit TimedNote(absl::Duration duration);
  ~TimedNote() override = default;

 protected:
  std::optional<float> GenerateNextNoteImpl(
      const SDL_AudioSpec& audio_spec) final;

  struct TimedNoteData {
    // How many samples the note will last for.
    uint64_t sample_lifetime_;

    // The total number of samples produced so far.
    uint64_t produced_samples_;
  };

  // Infallible version of `GenerateNextNote()`. Notes that override this class
  // should be able to generate sound forever.
  virtual float NextNote(const SDL_AudioSpec& audio_spec,
                         const TimedNoteData& data) = 0;

 private:
  absl::Duration duration_;
  std::optional<TimedNoteData> data_;
};

}  // namespace sdl
