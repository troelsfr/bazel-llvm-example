load("@pybind11_bazel//:build_defs.bzl", "pybind_extension")

pybind_extension(
    name = "pyqir",
    srcs = glob(["pyqir.cpp"]),
    deps=[
      "//qir/cc/qir-module:qir-module",
    ],
    visibility = ["//visibility:private"],
)


py_library(
    name = "pyqir",
    imports = [ ":pyqir.so" ],
    data = [":pyqir.so",],
    visibility = [
        "//visibility:public",
    ],
)
