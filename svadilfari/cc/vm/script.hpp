#pragma once
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "svadilfari/cc/llvm/llvm.hpp"
#include "svadilfari/cc/runtime/runtime.hpp"

#include <iostream>
#include <vector>

namespace compiler {

struct Script
{
  using LlvmContext = llvm::LLVMContext;
  using Module      = llvm::Module;
  using String      = std::string;
  enum Type
  {
    LL_SCRIPT,
    BC_SCRIPT
  };

  Type   type{Type::LL_SCRIPT};
  String name;
  String payload;

  std::unique_ptr<Module> load(LlvmContext *context) const
  {
    llvm::SMDiagnostic error;
    auto               ret = llvm::parseIR(llvm::MemoryBufferRef(payload, name), error, *context);

    return ret;
  }
};

}  // namespace compiler