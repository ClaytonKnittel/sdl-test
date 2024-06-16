cc_library(
    name = "audio_device",
    srcs = ["audio_device.cc"],
    hdrs = ["audio_device.h"],
    visibility = ["//visibility:public"],
    deps = [
        "@abseil-cpp//absl/status:statusor",
        "@abseil-cpp//absl/strings",
    ],
)

cc_library(
    name = "audio_buffer",
    srcs = ["audio_buffer.cc"],
    hdrs = ["audio_buffer.h"],
    visibility = ["//visibility:public"],
    deps = [
        "@abseil-cpp//absl/status:status",
        "@abseil-cpp//absl/strings:str_format",
    ],
)

cc_library(
    name = "note",
    srcs = ["note.cc"],
    hdrs = ["note.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":audio_buffer",
        "@abseil-cpp//absl/time",
    ],
)

cc_library(
    name = "sin_wave",
    srcs = ["sin_wave.cc"],
    hdrs = ["sin_wave.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":note",
        "@abseil-cpp//absl/time",
    ],
)

cc_library(
    name = "triangle_wave",
    srcs = ["triangle_wave.cc"],
    hdrs = ["triangle_wave.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":note",
        "@abseil-cpp//absl/time",
    ],
)

cc_library(
    name = "volume",
    srcs = ["volume.cc"],
    hdrs = ["volume.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":note",
    ],
)
