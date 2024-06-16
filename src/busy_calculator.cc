#include "src/busy_calculator.h"

#include "absl/log/log.h"
#include "absl/time/time.h"

namespace game {

BusyCalculator::BusyCalculator(absl::Time now) : last_log_time_(now) {}

void BusyCalculator::BeginFrame(absl::Time now) {
  begin_time_ = now;
}

void BusyCalculator::EndFrame(absl::Time now) {
  if (begin_time_ == absl::InfinitePast()) {
    LOG(ERROR) << "Called `BusyCalculator::EndFrame` before "
                  "`BusyCalculator::BeginFrame`";
    return;
  }

  absl::Duration frame_time = now - begin_time_;

  active_time_ += frame_time;
  absl::Duration time_since_last_log = now - last_log_time_;
  if (time_since_last_log >= absl::Seconds(1)) {
    LOG(INFO) << absl::FDivDuration(active_time_, time_since_last_log) * 100
              << "% active time";
    last_log_time_ = now;
    active_time_ = absl::ZeroDuration();
  }
}

}  // namespace game
