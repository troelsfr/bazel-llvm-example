#pragma once

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wunused-value"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wunknown-warning-option"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
#pragma clang diagnostic ignored "-Wpedantic"
#pragma clang diagnostic ignored "-Werror"
#pragma clang diagnostic ignored "-Wshadow"
#pragma clang diagnostic ignored "-Wreturn-std-move"
#pragma clang diagnostic ignored "-Wunknown-warning-option"
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"

// VM
// #include "llvm/ADT/StringRef.h"
// #include "llvm/ExecutionEngine/GenericValue.h"
// #include "llvm/ExecutionEngine/Interpreter.h"
// #include "llvm/ExecutionEngine/JITSymbol.h"
// #include "llvm/ExecutionEngine/Orc/CompileUtils.h"
// #include "llvm/ExecutionEngine/Orc/Core.h"
// #include "llvm/ExecutionEngine/Orc/ExecutionUtils.h"
// #include "llvm/ExecutionEngine/Orc/IRCompileLayer.h"
// #include "llvm/ExecutionEngine/Orc/JITTargetMachineBuilder.h"
// #include "llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h"
// #include "llvm/ExecutionEngine/Orc/TargetProcessControl.h"
// #include "llvm/ExecutionEngine/SectionMemoryManager.h"
// #include "llvm/IR/Constants.h"
// #include "llvm/IR/DataLayout.h"
// #include "llvm/IR/DerivedTypes.h"
// #include "llvm/IR/Instructions.h"
// #include "llvm/IR/LLVMContext.h"
// #include "llvm/IR/Module.h"
// #include "llvm/Support/TargetSelect.h"
// #include "llvm/Support/raw_ostream.h"

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif