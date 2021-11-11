
cc_library(
    name = "lib",
    srcs = glob(["*.cpp", "**/*.cpp"]),
    hdrs = glob(["*.hpp", "**/*.hpp"]),   
    visibility = ["//visibility:public"],
    deps = [
      "@llvm-project//llvm:IRReader",
      "@llvm-project//llvm:Target",
      "@llvm-project//llvm:TransformUtils",
      "@llvm-project//llvm:InstCombine",
      "@llvm-project//llvm:Scalar",

      # VM deps
      # Requires libpmf "@llvm-project//llvm:Exegesis",            
      # "@libpfm4//:pfm4",
      # "@llvm-project//llvm:ExecutionEngine",
      # "@llvm-project//llvm:OrcJIT"
    ],
)