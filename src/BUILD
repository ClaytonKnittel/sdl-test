cc_binary(
    name = "main",
    srcs = ["main.cc"],
    data = [
        "//res:imgs",
    ],
    deps = [
        ":busy_calculator",
        ":framerate_throttle",
        ":utils",
        "//src/sdl:rect",
        "//src/sdl:renderer",
        "//src/sdl:texture",
        "//src/sdl:window",
        "//src/sdl/sound:audio_buffer",
        "//src/sdl/sound:audio_device",
        "//src/sdl/sound:note",
        "//src/sdl/sound:sin_wave",
        "//src/sdl/sound:triangle_wave",
        "//src/sdl/sound:volume",
        "@abseil-cpp//absl/log",
        "@abseil-cpp//absl/log:globals",
        "@abseil-cpp//absl/log:initialize",
        "@abseil-cpp//absl/status:status",
        "@abseil-cpp//absl/status:statusor",
        "@abseil-cpp//absl/time",
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
    name = "framerate_throttle",
    srcs = ["framerate_throttle.cc"],
    hdrs = ["framerate_throttle.h"],
    deps = [
        "@abseil-cpp//absl/log",
        "@abseil-cpp//absl/time",
    ],
)

cc_library(
    name = "busy_calculator",
    srcs = ["busy_calculator.cc"],
    hdrs = ["busy_calculator.h"],
    deps = [
        "@abseil-cpp//absl/log",
        "@abseil-cpp//absl/time",
    ],
)
