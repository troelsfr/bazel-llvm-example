#pragma once
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "qir/cc/llvm/llvm.hpp"
#include "qir/cc/qir-module/value-container.hpp"
#include "qir/cc/runtime/runtime-type.hpp"

#include <type_traits>
#include <typeindex>
namespace compiler {

class TypedValue;
class TypedValuePrototype
{
public:
  using Value                  = llvm::Value;
  using Constant               = llvm::Constant;
  using LLVMContext            = llvm::LLVMContext;
  using Builder                = llvm::IRBuilder<>;
  using TypedValuePrototypePtr = ValueContianer<TypedValuePrototype>;

  using Assigned = void;

  TypedValuePrototype(TypeDeclaration const &type, llvm::IRBuilder<> &builder);
  virtual ~TypedValuePrototype() = default;

  virtual Constant *toConstant(LLVMContext *context, Builder &builder) const;

  virtual Value      *toValue(LLVMContext *context, Builder &builder) const = 0;
  virtual std::string toString() const                                      = 0;
  virtual bool        isLhs() const;

  std::type_index        typeId() const;
  TypeDeclaration const &typeDeclaration() const;
  llvm::IRBuilder<>     &builder() const
  {
    return builder_;
  }

  virtual void                   writeValue(Value *);
  virtual Value                 *readValue();
  virtual Constant              *readConstant();
  virtual TypedValuePrototypePtr getArrayElement(TypedValuePrototypePtr const &index);

protected:
  TypeDeclaration    type_decl_;
  llvm::IRBuilder<> &builder_;

  Value    *value_{nullptr};
  Constant *constant_{nullptr};
};
using TypedValuePrototypePtr = TypedValuePrototype::TypedValuePrototypePtr;
}  // namespace compiler
