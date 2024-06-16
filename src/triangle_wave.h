#pragma once

#include <SDL2/SDL_audio.h>

#include "absl/time/time.h"

#include "src/note.h"
namespace sdl {

class TriangleWave : public TimedNote {
 public:
  TriangleWave(float hz, absl::Duration duration);
  ~TriangleWave() override = default;

 protected:
  float NextNote(const SDL_AudioSpec& audio_spec,
                 const TimedNoteData& data) override;

 private:
  float hz_;
};

}  // namespace sdl
