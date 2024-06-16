#pragma once

#include <memory>
#include <optional>

#include <SDL2/SDL_audio.h>

#include "src/sdl/sound/note.h"

namespace sdl {

class Volume : public Note {
 public:
  Volume(float pct, std::unique_ptr<TimedNote> note);
  ~Volume() override = default;

 protected:
  std::optional<float> GenerateNextNoteImpl(
      const SDL_AudioSpec& audio_spec) override;

 private:
  float pct_;
  std::unique_ptr<TimedNote> note_;
};

}  // namespace sdl
