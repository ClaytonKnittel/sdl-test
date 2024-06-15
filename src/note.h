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

  // Generates the next sample for the note.
  std::optional<float> GenerateNextNote();

  // Fills up the buffer with sound data. If `true` is returned, that means the
  // note was fully consumed and will not produce any more sound data.
  bool PopulateBuffer(AudioBuffer& buffer);

 protected:
  // Returns the next sample for the note, advancing the internal state of the
  // note generator.
  virtual std::optional<float> GenerateNextNoteImpl() = 0;

 private:
  bool done_ = false;
};

class TimedNote : public Note {
 public:
  TimedNote(absl::Duration duration, const SDL_AudioSpec& audio_spec);
  ~TimedNote() override = default;

 protected:
  std::optional<float> GenerateNextNoteImpl() final;

  // Infallible version of `GenerateNextNote()`. Notes that override this class
  // should be able to generate sound forever.
  virtual float NextNote() = 0;

  // How many samples the note will last for.
  uint64_t sample_lifetime_;

  // The total number of samples produced so far.
  uint64_t produced_samples_;

  // The frequency of the audio samples (per second).
  float frequency_;
};

}  // namespace sdl
