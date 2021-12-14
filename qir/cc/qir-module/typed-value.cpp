// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "qir/cc/qir-module/typed-value.hpp"

#include "qir/cc/llvm/llvm.hpp"
#include "qir/cc/qir-module/value-container.hpp"

#include <type_traits>
#include <typeindex>
namespace compiler {

TypedValuePtr TypedValue::create(TypeDeclaration const &type_decl, llvm::IRBuilder<> &builder,
                                 llvm::Value *value)
{
  TypedValuePtr ret;
  ret.reset(new TypedValue(type_decl, builder, value));
  return ret;
}

TypedValue::Value *TypedValue::toValue(LLVMContext *context, Builder &builder) const
{
  return value_;
}

std::string TypedValue::toString() const
{
  return "Value";
}

TypedValue::TypedValue(TypeDeclaration const &type_decl, llvm::IRBuilder<> &builder,
                       llvm::Value *value)
  : TypedValuePrototype(type_decl, builder)
  , value_{value}
{}

}  // namespace compiler