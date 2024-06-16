cc_library(
    name = "window",
    srcs = ["window.cc"],
    hdrs = ["window.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":sdl_utils",
        "@abseil-cpp//absl/status:statusor",
        "@abseil-cpp//absl/strings",
    ],
)

cc_library(
    name = "renderer",
    srcs = ["renderer.cc"],
    hdrs = ["renderer.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":window",
        "@abseil-cpp//absl/status:status",
        "@abseil-cpp//absl/status:statusor",
        "@abseil-cpp//absl/strings",
    ],
)

cc_library(
    name = "texture",
    srcs = ["texture.cc"],
    hdrs = ["texture.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":renderer",
        "@abseil-cpp//absl/status:statusor",
        "@abseil-cpp//absl/strings",
    ],
)

cc_library(
    name = "sdl_utils",
    srcs = ["sdl_utils.cc"],
    hdrs = ["sdl_utils.h"],
    deps = [
    ],
)
