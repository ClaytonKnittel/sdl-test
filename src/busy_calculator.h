#pragma once

#include "absl/time/time.h"

namespace game {

class BusyCalculator {
 public:
  explicit BusyCalculator(absl::Time now);

  void BeginFrame(absl::Time now);

  void EndFrame(absl::Time now);

 private:
  absl::Time begin_time_;

  // The time of the last log.
  absl::Time last_log_time_;
  // The total amount of time spent not sleeping since the last log.
  absl::Duration active_time_;
};

}  // namespace game
