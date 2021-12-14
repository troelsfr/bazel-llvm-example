cc_library(
    name = "script-builder",
    srcs = glob(["*.cpp"]),
    hdrs = glob(["*.hpp"]),    
    visibility = ["//visibility:public"],
    deps = [
      "//svadilfari/cc/llvm:lib",
      "//svadilfari/cc/runtime:lib",
      "//svadilfari/cc/vm:lib",
      "//svadilfari/rust/logger:logger",
    ],
)
