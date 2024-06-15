#pragma once

#include <memory>
#include <optional>

#include "src/note.h"

namespace sdl {

class Volume : public Note {
 public:
  Volume(float pct, std::unique_ptr<TimedNote> note);

  std::optional<float> GenerateNextNote() override;

 private:
  float pct_;
  std::unique_ptr<TimedNote> note_;
};

}  // namespace sdl
