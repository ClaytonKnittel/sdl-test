#pragma once

#include <SDL2/SDL_audio.h>

#include "absl/time/time.h"

#include "src/note.h"

namespace sdl {

class SinWave : public TimedNote {
 public:
  SinWave(float hz, absl::Duration duration);
  ~SinWave() override = default;

 protected:
  float NextNote(const SDL_AudioSpec& audio_spec,
                 const TimedNoteData& data) override;

 private:
  // The frequency of the note * 2π.
  float hz_2pi_;
};

}  // namespace sdl
