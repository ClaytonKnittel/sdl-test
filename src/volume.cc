#include "src/volume.h"

#include <memory>
#include <optional>

#include <SDL2/SDL_audio.h>

#include "src/note.h"

namespace sdl {

Volume::Volume(float pct, std::unique_ptr<TimedNote> note)
    : pct_(pct), note_(std::move(note)) {}

std::optional<float> Volume::GenerateNextNoteImpl(
    const SDL_AudioSpec& audio_spec) {
  auto next_note = note_->GenerateNextNote(audio_spec);
  if (!next_note.has_value()) {
    return std::nullopt;
  }
  return pct_ * next_note.value();
}

}  // namespace sdl
