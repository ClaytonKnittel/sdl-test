#include "src/audio_buffer.h"

#include <cstdint>

#include <SDL2/SDL_stdinc.h>

#include "absl/status/status.h"
#include "absl/strings/str_format.h"

namespace sdl {

AudioBuffer::AudioBuffer(Uint8* raw_buffer, uint32_t stream_size, int channels)
    : buffer_(reinterpret_cast<float*>(raw_buffer)),
      num_samples_(stream_size / (channels * sizeof(float))),
      channels_(channels) {
  SDL_memset(buffer_, 0, stream_size);
}

void AudioBuffer::AudioBufferIterator::WriteAll(float sample) {
  for (int channel = 0; channel < parent_.channels_; channel++) {
    Write(sample, channel);
  }
}

void AudioBuffer::AudioBufferIterator::Write(float sample, int channel) {
  parent_.buffer_[sample_idx_ * parent_.channels_ + channel] += sample;
}

void AudioBuffer::AudioBufferIterator::Write2Channel(float left, float right) {
  Write(left, 0);
  Write(right, 1);
}

AudioBuffer::AudioBufferIterator::AudioBufferIterator(AudioBuffer& parent,
                                                      uint32_t sample_idx)
    : parent_(parent), sample_idx_(sample_idx) {}

AudioBuffer::AudioBufferIterator AudioBuffer::begin() {
  return AudioBuffer::AudioBufferIterator(*this, 0);
}

AudioBuffer::AudioBufferIterator AudioBuffer::end() {
  return AudioBuffer::AudioBufferIterator(*this, num_samples_);
}

absl::Status AudioBuffer::TranslateBuffer(SDL_AudioFormat format) {
  switch (format) {
    // This is the format the buffer is in.
    case AUDIO_F32LSB:
      break;
    case AUDIO_S32LSB: {
      for (uint32_t i = 0; i < num_samples_ * channels_; i++) {
        reinterpret_cast<int32_t*>(buffer_)[i] =
            (2 * buffer_[i] - 1) * static_cast<float>(INT32_MAX);
      }
    }
    default: {
      return absl::InternalError(
          absl::StrFormat("Unimplemented audio buffer format: %04x", format));
    }
  }

  return absl::OkStatus();
}

}  // namespace sdl
