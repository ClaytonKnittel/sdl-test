#pragma once

#include <SDL2/SDL_audio.h>

#include "absl/time/time.h"

#include "src/note.h"

namespace sdl {

class SinWave : public Note {
 public:
  SinWave(float amp, float hz, absl::Duration duration,
          const SDL_AudioSpec& audio_spec);

 protected:
  float GenerateNextNote() override;

 private:
  float amp_;
  float hz_;
};

}  // namespace sdl
