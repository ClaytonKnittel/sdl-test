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
        ":drawable",
        ":window",
        "@abseil-cpp//absl/container:flat_hash_map",
        "@abseil-cpp//absl/status",
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
    name = "drawable",
    hdrs = ["drawable.h"],
    visibility = ["//visibility:public"],
    deps = [
    ],
)

cc_library(
    name = "rect",
    srcs = ["rect.cc"],
    hdrs = ["rect.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":drawable",
    ],
)

cc_library(
    name = "sdl_utils",
    srcs = ["sdl_utils.cc"],
    hdrs = ["sdl_utils.h"],
    deps = [
    ],
)
