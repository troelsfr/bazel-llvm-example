load("@rules_rust//rust:rust.bzl", "rust_binary")

rust_binary(
    name = "hello-world",
    srcs = ["main.rs"],
    deps = ["//qir/rust/libtest:hello_lib"],
)
