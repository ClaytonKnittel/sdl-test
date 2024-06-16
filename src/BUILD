cc_binary(
  name = "main",
  srcs = ["main.cc"],
  data = [
    "//res:imgs",
  ],
  deps = [
    ":audio_buffer",
    ":audio_device",
    ":framerate_throttle",
    ":note",
    ":renderer",
    ":sin_wave",
    ":texture",
    ":triangle_wave",
    ":utils",
    ":volume",
    ":window",
    "@abseil-cpp//absl/log",
    "@abseil-cpp//absl/log:globals",
    "@abseil-cpp//absl/log:initialize",
    "@abseil-cpp//absl/status:status",
    "@abseil-cpp//absl/status:statusor",
    "@abseil-cpp//absl/time",
  ],
)

cc_library(
  name = "window",
  hdrs = ["window.h"],
  srcs = ["window.cc"],
  deps = [
    ":sdl_utils",
    "@abseil-cpp//absl/status:statusor",
    "@abseil-cpp//absl/strings",
  ],
)

cc_library(
  name = "renderer",
  hdrs = ["renderer.h"],
  srcs = ["renderer.cc"],
  deps = [
    ":window",
    "@abseil-cpp//absl/status:status",
    "@abseil-cpp//absl/status:statusor",
    "@abseil-cpp//absl/strings",
  ],
)

cc_library(
  name = "texture",
  hdrs = ["texture.h"],
  srcs = ["texture.cc"],
  deps = [
    ":renderer",
    "@abseil-cpp//absl/status:statusor",
    "@abseil-cpp//absl/strings",
  ],
)

cc_library(
  name = "utils",
  hdrs = ["utils.h"],
  deps = [
    "@abseil-cpp//absl/status",
    "@abseil-cpp//absl/status:statusor",
    "@abseil-cpp//absl/strings",
    "@googletest//:gtest",
  ],
)

cc_library(
  name = "sdl_utils",
  hdrs = ["sdl_utils.h"],
  srcs = ["sdl_utils.cc"],
  deps = [
  ],
)

cc_library(
  name = "audio_device",
  hdrs = ["audio_device.h"],
  srcs = ["audio_device.cc"],
  deps = [
    "@abseil-cpp//absl/status:statusor",
    "@abseil-cpp//absl/strings",
  ],
)

cc_library(
  name = "audio_buffer",
  hdrs = ["audio_buffer.h"],
  srcs = ["audio_buffer.cc"],
  deps = [
    "@abseil-cpp//absl/status:status",
    "@abseil-cpp//absl/strings:str_format",
  ],
)

cc_library(
  name = "note",
  hdrs = ["note.h"],
  srcs = ["note.cc"],
  deps = [
    ":audio_buffer",
    "@abseil-cpp//absl/time",
  ],
)

cc_library(
  name = "sin_wave",
  hdrs = ["sin_wave.h"],
  srcs = ["sin_wave.cc"],
  deps = [
    ":note",
    "@abseil-cpp//absl/time",
  ],
)

cc_library(
  name = "triangle_wave",
  hdrs = ["triangle_wave.h"],
  srcs = ["triangle_wave.cc"],
  deps = [
    ":note",
    "@abseil-cpp//absl/time",
  ],
)

cc_library(
  name = "volume",
  hdrs = ["volume.h"],
  srcs = ["volume.cc"],
  deps = [
    ":note",
  ],
)

cc_library(
  name = "framerate_throttle",
  hdrs = ["framerate_throttle.h"],
  srcs = ["framerate_throttle.cc"],
  deps = [
    "@abseil-cpp//absl/log",
    "@abseil-cpp//absl/time",
  ],
)
