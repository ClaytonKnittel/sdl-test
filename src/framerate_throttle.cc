#include "src/framerate_throttle.h"

#include <SDL2/SDL_timer.h>
#include <cstdint>

#include "absl/log/log.h"
#include "absl/time/time.h"

namespace game {

FramerateThrottle::FramerateThrottle(uint32_t target_fps, absl::Time now)
    : begin_time_(absl::InfinitePast()),
      target_duration_(absl::Seconds(1) / target_fps),
      last_log_time_(now),
      framerate_count_(0) {}

void FramerateThrottle::BeginFrame(absl::Time now) {
  begin_time_ = now;
}

void FramerateThrottle::EndFrame(absl::Time now) {
  if (begin_time_ == absl::InfinitePast()) {
    LOG(ERROR) << "Called `FramerateThrottle::EndFrame` before "
                  "`FramerateThrottle::BeginFrame`";
    return;
  }

  absl::Duration frame_time = now - begin_time_;

  framerate_count_++;
  active_time_ += frame_time;
  absl::Duration time_since_last_log = now - last_log_time_;
  if (time_since_last_log >= absl::Seconds(1)) {
    LOG(INFO) << framerate_count_ / absl::ToDoubleSeconds(time_since_last_log)
              << " fps, "
              << absl::FDivDuration(active_time_, time_since_last_log) * 100
              << "% active time";
    last_log_time_ = now;
    framerate_count_ = 0;
    active_time_ = absl::ZeroDuration();
  }

  // If the time spent on this frame is less than the target time per frame,
  // then we can wait the remainder of the target frame time.
  if (frame_time + absl::Milliseconds(1) <= target_duration_) {
    absl::Duration remaining_time_in_frame = target_duration_ - frame_time;
    SDL_Delay(remaining_time_in_frame / absl::Milliseconds(1));
  }

  begin_time_ = absl::InfinitePast();
}

}  // namespace game
