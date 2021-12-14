#pragma once
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "svadilfari/cc/llvm/llvm-helpers.hpp"
#include "svadilfari/cc/runtime/runtime-type.hpp"
#include "svadilfari/cc/runtime/size-of.hpp"

#include <unordered_map>
namespace compiler {
class TypeDeclarationRegister
{
public:
  using String      = std::string;
  using LlvmType    = TypeDeclaration::LlvmType;
  using LlvmContext = llvm::LLVMContext;

  enum
  {
    FIRST_USER_TYPE_ID = 30
  };

  TypeDeclarationRegister();

  template <typename T>
  void declareNativeType(String const &name, LlvmType *llvm_type = nullptr);

  TypeDeclaration getType(uint64_t const &id);
  TypeDeclaration getType(String const &name);
  TypeDeclaration getType(std::type_index const &type_id);

private:
  std::unordered_map<uint64_t, TypeDeclaration>        types_by_id_{};
  std::unordered_map<String, TypeDeclaration>          types_by_name_{};
  std::unordered_map<std::type_index, TypeDeclaration> types_by_native_id_{};

  uint64_t type_id_counter_{FIRST_USER_TYPE_ID};
};

template <typename T>
void TypeDeclarationRegister::declareNativeType(String const &name, LlvmType *llvm_type)
{
  // Creating the type and adding it to the register
  TypeDeclaration new_type{type_id_counter_, llvm_type, std::type_index(typeid(T)),
                           details::SizeOf<T>::value(), name};

  // Creating an initiator to allow
  new_type.initiator = [](LlvmContext *context, String const &name) {
    return llvm_helpers::LLVMTypeAllocator<T>::type(*context, name);
  };

  types_by_id_[new_type.id];
  types_by_name_[name] = new_type;
  types_by_native_id_[new_type.native_type_id];

  ++type_id_counter_;
}

}  // namespace compiler