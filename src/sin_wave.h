#pragma once

#include <SDL2/SDL_audio.h>

#include "absl/time/time.h"

#include "src/note.h"

namespace sdl {

class SinWave : public TimedNote {
 public:
  SinWave(float hz, absl::Duration duration, const SDL_AudioSpec& audio_spec);
  ~SinWave() override = default;

 protected:
  float NextNote() override;

 private:
  float hz_;
};

}  // namespace sdl
