#pragma once
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "svadilfari/cc/llvm/llvm.hpp"

#include <cstring>
#include <vector>

namespace compiler {

struct FunctionDeclaration
{
  using String       = std::string;
  using Function     = llvm::Function;
  using ArgTypeNames = std::vector<String>;

  Function    *function{nullptr};
  String       name{};
  String       return_type{};
  ArgTypeNames argument_types{};
};

}  // namespace compiler