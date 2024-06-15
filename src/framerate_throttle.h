#pragma once

#include <cstdint>

#include "absl/time/time.h"

namespace game {

class FramerateThrottle {
 public:
  FramerateThrottle(uint32_t target_fps, absl::Time now);

  void BeginFrame(absl::Time now);

  void EndFrame(absl::Time now);

 private:
  absl::Time begin_time_;

  absl::Duration target_duration_;

  // The time of the last framerate log.
  absl::Time last_log_time_;
  // The count of frames rendered since the last framerate log.
  uint64_t framerate_count_;
  // The total amount of time spent not sleeping since the last framerate log.
  absl::Duration active_time_;
};

}  // namespace game
