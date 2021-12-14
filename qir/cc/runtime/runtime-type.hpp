#pragma once
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "qir/cc/llvm/llvm.hpp"

#include <cstring>
#include <functional>
#include <typeindex>
#include <vector>

namespace compiler {

struct TypeDeclaration
{
  using String        = std::string;
  using LlvmType      = llvm::Type;
  using LlvmContext   = llvm::LLVMContext;
  using LlvmInitiator = std::function<LlvmType *(LlvmContext *, String const &)>;

  uint64_t        id{0};
  LlvmType       *llvm_type{nullptr};
  std::type_index native_type_id{std::type_index(typeid(nullptr_t))};
  uint64_t        size{0};
  String          name{""};

  //
  LlvmInitiator initiator{nullptr};

  bool isNative() const
  {
    return native_type_id != std::type_index(typeid(nullptr_t));
  }
};

struct TypeDefinition
{
  using Constructor = std::function<void *()>;
  using Destructor  = std::function<void(void *)>;
  using IncreaseRef = std::function<void(void *)>;
  using DecreaseRef = std::function<void(void *)>;
  using ChangeRef   = std::function<void(void *, int64_t)>;

  uint64_t id{0};

  // TODO: Move to runtime
  // Allocation and freeing
  Constructor constructor{nullptr};
  Destructor  destructor{nullptr};

  // Reference counting
  IncreaseRef increase_reference_count{nullptr};
  DecreaseRef decrease_reference_count{nullptr};
  ChangeRef   change_reference_count{nullptr};

  bool isPod() const
  {
    return constructor == nullptr && destructor == nullptr;
  }
};

}  // namespace compiler