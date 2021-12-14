cc_library(
    name = "qir-module",
    srcs = glob(["*.cpp"]),
    hdrs = glob(["*.hpp"]),    
    visibility = ["//visibility:public"],
    deps = [
      "//svadilfari/cc/llvm:lib",
      "//svadilfari/cc/runtime:lib",

      "//svadilfari/rust/logger:logger",
    ],
)
