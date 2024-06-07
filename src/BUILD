cc_binary(
  name = "main",
  srcs = ["main.cc"],
  data = [
    "//res:imgs",
  ],
  deps = [
    ":utils",
    "@abseil-cpp//absl/log",
    "@abseil-cpp//absl/log:globals",
    "@abseil-cpp//absl/log:initialize",
    "@abseil-cpp//absl/status:status",
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
