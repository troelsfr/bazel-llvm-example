// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "svadilfari/cc/runtime/runtime.hpp"

#include <iostream>

namespace compiler {
Runtime::Runtime() = default;

void Runtime::initPlatform()
{
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::InitializeNativeTargetAsmParser();

  defineFunction("print", [](int64_t n) { std::cout << "PRINTER: " << n << std::endl; });
}
}  // namespace compiler