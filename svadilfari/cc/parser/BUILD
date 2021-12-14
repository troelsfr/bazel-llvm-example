load("@rules_antlr//antlr:antlr4.bzl", "antlr")


antlr(
    name = "generated",
    srcs = ["ToyLexer.g4", "ToyParser.g4"],
    package = "toylang",
    visitor = True,
    language = "Cpp",
    visibility = ["//visibility:public"],
)


cc_library(
    name = "lib",
    srcs = [":generated"],
    visibility = ["//visibility:public"],
    deps = [
        ":generated",
        "@antlr4_runtimes//:cpp",
    ],
)


cc_binary(
    name = "parser",
    srcs = glob(["*.cpp"]) ,
    deps = [ 
      ":lib",      
      "//svadilfari/cc/llvm:lib",
      "//svadilfari/cc/qir-module:qir-module",
      "//svadilfari/cc/vm:lib",

      "@antlr4_runtimes//:cpp",
    ],
    visibility = [
        "//visibility:public",
    ],
)
