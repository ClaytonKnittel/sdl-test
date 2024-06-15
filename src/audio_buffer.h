#pragma once

#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_stdinc.h>

#include "absl/status/status.h"

namespace sdl {

class AudioBuffer {
 public:
  // Initializes an audio buffer to wrap the given raw buffer, zeroing the whole
  // thing out.
  AudioBuffer(Uint8* raw_buffer, uint32_t stream_size, int channels);

  class AudioBufferIterator {
    friend class AudioBuffer;

   public:
    // Writes/accumulates the sample to all channels of the audio buffer.
    void WriteAll(float sample);

    // Writes/accumulates the sample to the given channel of the audio buffer.
    void Write(float sample, int channel);

    // Writes the samples to the two audio channels, left and right
    // respectively.
    void Write2Channel(float left, float right);

    AudioBufferIterator& operator++() {
      sample_idx_++;
      return *this;
    }

    bool operator==(const AudioBufferIterator& other) const {
      return sample_idx_ == other.sample_idx_;
    }

    bool operator!=(const AudioBufferIterator& other) const {
      return !(*this == other);
    }

   private:
    AudioBufferIterator(AudioBuffer& parent, uint32_t sample_idx);

    AudioBuffer& parent_;
    uint32_t sample_idx_;
  };

  AudioBufferIterator begin();
  AudioBufferIterator end();

  // Overwrites the whole buffer, translating it to the given format. The audio
  // buffer is unusable after this.
  absl::Status TranslateBuffer(SDL_AudioFormat format);

 private:
  float* buffer_;
  uint32_t num_samples_;
  int channels_;
};

}  // namespace sdl
