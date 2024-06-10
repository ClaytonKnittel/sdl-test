cc_binary(
  name = "main",
  srcs = ["main.cc"],
  data = [
    "//res:imgs",
  ],
  deps = [
    ":utils",
    ":window",
    ":renderer",
    ":texture",
    "@abseil-cpp//absl/log",
    "@abseil-cpp//absl/log:globals",
    "@abseil-cpp//absl/log:initialize",
    "@abseil-cpp//absl/status:status",
    "@abseil-cpp//absl/status:statusor",
    "@abseil-cpp//absl/strings",
  ],
)

cc_library(
  name = "window",
  hdrs = ["window.h"],
  srcs = ["window.cc"],
  deps = [
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
