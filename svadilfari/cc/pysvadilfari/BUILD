load("@pybind11_bazel//:build_defs.bzl", "pybind_extension")

pybind_extension(
    name = "pysvadilfari",
    srcs = glob(["pysvadilfari.cpp"]),
    deps=[
      "//svadilfari/cc/qir-module:qir-module",
      "//svadilfari/cc/runtime:lib",
      "//svadilfari/cc/vm:lib",
    ],
    visibility = ["//visibility:private"],
)


py_library(
    name = "pysvadilfari",
    imports = [ ":pysvadilfari.so" ],
    data = [":pysvadilfari.so",],
    visibility = [
        "//visibility:public",
    ],
)
