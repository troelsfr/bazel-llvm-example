
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
# Use this for other architectures
#        "@llvm-project//llvm:AllTargetsAsmParsers",
#        "@llvm-project//llvm:AllTargetsCodeGens",
#        "@llvm-project//llvm:AllTargetsDisassemblers",

        "@llvm-project//llvm:X86AsmParser",
        "@llvm-project//llvm:X86CodeGen",
        "@llvm-project//llvm:X86Disassembler",

        "@llvm-project//llvm:AsmParser",
        "@llvm-project//llvm:BitReader",
        "@llvm-project//llvm:CodeGen",
        "@llvm-project//llvm:ExecutionEngine",
        "@llvm-project//llvm:Interpreter",
        "@llvm-project//llvm:MCJIT",
        "@llvm-project//llvm:Object",
        "@llvm-project//llvm:OrcJIT",
        "@llvm-project//llvm:Support",
        "@llvm-project//llvm:config",
    ],
)