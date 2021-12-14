load("@pybind11_bazel//:build_defs.bzl", "pybind_extension")

pybind_extension(
    name = "pyllvm",
    srcs = glob(["pyllvm.cpp"]),
    deps=[
      "//qir/cc/qir-module:qir-module",
      "//qir/cc/runtime:lib",
      "//qir/cc/vm:lib",
    ],
    visibility = ["//visibility:private"],
)


py_library(
    name = "pyllvm",
    imports = [ ":pyllvm.so" ],
    data = [":pyllvm.so",],
    visibility = [
        "//visibility:public",
    ],
)
