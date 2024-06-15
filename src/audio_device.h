#pragma once

#include <memory>
#include <optional>

#include "SDL2/SDL_audio.h"
#include "SDL2/SDL_stdinc.h"
#include "absl/status/statusor.h"

namespace sdl {

struct AudioDeviceCallbackData;

class AudioDevice {
  friend class AudioDeviceBuilder;

 public:
  using CallbackFn =
      std::function<void(void* udata, Uint8* stream, int stream_len,
                         const SDL_AudioSpec& audio_spec)>;

  AudioDevice(AudioDevice&&) noexcept;
  ~AudioDevice();

  AudioDevice& operator=(AudioDevice&&) = delete;

 private:
  AudioDevice(SDL_AudioDeviceID device_id,
              std::unique_ptr<AudioDeviceCallbackData> data_);

  std::optional<SDL_AudioDeviceID> device_id_;
  std::unique_ptr<AudioDeviceCallbackData> data_;
};

class AudioDeviceBuilder {
 public:
  AudioDeviceBuilder() = default;

  AudioDeviceBuilder& WithDevice(const char* device_name);
  AudioDeviceBuilder& WithFrequency(int frequency);
  AudioDeviceBuilder& WithFormat(std::optional<SDL_AudioFormat> format);
  AudioDeviceBuilder& WithChannels(std::optional<Uint8> channels);
  AudioDeviceBuilder& WithSamples(std::optional<Uint16> samples);
  AudioDeviceBuilder& WithCallback(
      std::optional<AudioDevice::CallbackFn> callback);
  AudioDeviceBuilder& WithUserdata(std::optional<void*> userdata);

  absl::StatusOr<AudioDevice> Build();

 private:
  // The name of the audio device to open. If not specified, SDL will choose a
  // reasonable default.
  std::optional<const char*> device_name_;

  // DSP frequency -- samples per second (REQUIRED).
  std::optional<int> frequency_;

  // Audio data format (REQUIRED).
  std::optional<SDL_AudioFormat> format_;

  // Number of channels: 1 mono, 2 stereo (REQUIRED).
  std::optional<Uint8> channels_;

  // Audio buffer size in sample FRAMES (total samples divided by channel
  // count) (REQUIRED).
  std::optional<Uint16> samples_;

  // Callback that feeds the audio device (REQUIRED).
  std::optional<AudioDevice::CallbackFn> callback_;

  // If `callback_` is defined, the user data to pass to the callback.
  std::optional<void*> userdata_;
};

}  // namespace sdl
