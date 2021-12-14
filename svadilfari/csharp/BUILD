load("@io_bazel_rules_dotnet//dotnet:defs.bzl", "csharp_library", "csharp_binary")
csharp_binary(
    name = "hello-world.exe",
    srcs = ["hello-world.cs"],
    nowarn = ["CS1701"],
    deps = [
        "@core_sdk_stdlib//:Microsoft.AspNetCore.App",
        "@core_sdk_stdlib//:libraryset",
        "@xunit.assert//:lib",
        "@xunit.extensibility.core//:lib",
        "@xunit.extensibility.execution//:lib",
    ],
)
