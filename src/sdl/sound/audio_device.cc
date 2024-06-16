#include "src/sdl/sound/audio_device.h"

#include <memory>

#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_stdinc.h>

#include "absl/status/statusor.h"
#include "absl/strings/str_cat.h"

namespace sdl {

struct AudioDeviceCallbackData {
  SDL_AudioSpec audio_spec;
  AudioDevice::CallbackFn callback;
  void* udata;
};

namespace {

void SDLCALL AudioDeviceCallback(void* udata, Uint8* stream, int stream_len) {
  auto* data = static_cast<AudioDeviceCallbackData*>(udata);
  data->callback(data->udata, stream, stream_len, data->audio_spec);
}

}  // namespace

AudioDevice::AudioDevice(AudioDevice&& audio_device) noexcept
    : device_id_(audio_device.device_id_),
      data_(std::move(audio_device.data_)) {
  audio_device.device_id_.reset();
}

AudioDevice::~AudioDevice() {
  if (device_id_.has_value()) {
    SDL_CloseAudioDevice(device_id_.value());
  }
}

AudioDevice::AudioDevice(SDL_AudioDeviceID device_id,
                         std::unique_ptr<AudioDeviceCallbackData> data)
    : device_id_(std::move(device_id)), data_(std::move(data)) {
  Unpause();
}

void AudioDevice::Pause() {
  SDL_PauseAudioDevice(device_id_.value(), 1);
}

void AudioDevice::Unpause() {
  SDL_PauseAudioDevice(device_id_.value(), 0);
}

AudioDeviceBuilder& AudioDeviceBuilder::WithDevice(const char* device_name) {
  device_name_ = device_name;
  return *this;
}

AudioDeviceBuilder& AudioDeviceBuilder::WithFrequency(int frequency) {
  frequency_ = frequency;
  return *this;
}

AudioDeviceBuilder& AudioDeviceBuilder::WithFormat(SDL_AudioFormat format) {
  format_ = format;
  return *this;
}

AudioDeviceBuilder& AudioDeviceBuilder::WithChannels(Uint8 channels) {
  channels_ = channels;
  return *this;
}

AudioDeviceBuilder& AudioDeviceBuilder::WithSamples(Uint16 samples) {
  samples_ = samples;
  return *this;
}

AudioDeviceBuilder& AudioDeviceBuilder::WithCallback(
    AudioDevice::CallbackFn callback) {
  callback_ = std::move(callback);
  return *this;
}

AudioDeviceBuilder& AudioDeviceBuilder::WithUserdata(void* userdata) {
  userdata_ = userdata;
  return *this;
}

absl::StatusOr<AudioDevice> AudioDeviceBuilder::Build() {
  if (!format_.has_value()) {
    return absl::InternalError("Cannot build AudioDevice without `format`.");
  }
  if (!channels_.has_value()) {
    return absl::InternalError("Cannot build AudioDevice without `channels`.");
  }
  if (!samples_.has_value()) {
    return absl::InternalError("Cannot build AudioDevice without `samples`.");
  }
  if (!callback_.has_value()) {
    return absl::InternalError("Cannot build AudioDevice without `callback`.");
  }

  auto data = std::make_unique<AudioDeviceCallbackData>(AudioDeviceCallbackData{
      .callback = callback_.value(),
      .udata = userdata_.value_or(nullptr),
  });

  SDL_AudioSpec desired = {
    .format = format_.value(),
    .channels = channels_.value(),
    .samples = samples_.value(),
    .callback = AudioDeviceCallback,
    .userdata = data.get(),
  };

  SDL_AudioDeviceID device_id =
      SDL_OpenAudioDevice(device_name_.value_or(nullptr), 0, &desired,
                          &data->audio_spec, SDL_AUDIO_ALLOW_ANY_CHANGE);
  if (device_id == 0) {
    return absl::InternalError(
        absl::StrCat("Failed to open audio device: ", SDL_GetError()));
  }

  std::cout << data->audio_spec.format << " "
            << static_cast<int>(data->audio_spec.channels) << " "
            << data->audio_spec.samples << std::endl;

  return AudioDevice(device_id, std::move(data));
}

}  // namespace sdl
