#pragma once
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "qir/cc/llvm/llvm.hpp"
#include "qir/cc/qir-module/typed-value-prototype.hpp"

#include <type_traits>
#include <typeindex>

namespace compiler {

class ConstantArray : public TypedValuePrototype
{
public:
  using ValueList = std::vector<llvm::Constant *>;

  template <typename T>
  static ValueContianer<ConstantArray> createNew(llvm::IRBuilder<> &builder,
                                                 llvm::Type *element_type, ValueList const &values)
  {

    // ConstantArray::get
    ValueContianer<ConstantArray> ret;
    // TODO: ret.reset(new ConstantArray(std::type_index(typeid(T)), builder, element_type,
    // values));
    return ret;
  }

  static ValueContianer<ConstantArray> createNew(TypeDeclaration const &type_decl,
                                                 llvm::IRBuilder<>     &builder,
                                                 llvm::Type *element_type, ValueList const &values)
  {
    ValueContianer<ConstantArray> ret;
    ret.reset(new ConstantArray(type_decl, builder, element_type, values));
    return ret;
  }
  virtual ~ConstantArray() = default;

  Constant *toConstant(LLVMContext *context, Builder &builder) const override
  {
    for (auto const &v : values_)
    {
      llvm::errs() << " - " << *v << "\n";
    }
    auto module = builder.GetInsertBlock()->getModule();
    auto c = llvm::ConstantArray::get(llvm::ArrayType::get(element_type_, values_.size()), values_);
    return new llvm::GlobalVariable(*module, type_, true, llvm::GlobalValue::InternalLinkage, c,
                                    "helloTest");
  }

  Value *toValue(LLVMContext *context, Builder &builder) const override
  {
    auto module = builder.GetInsertBlock()->getModule();
    auto c = llvm::ConstantArray::get(llvm::ArrayType::get(element_type_, values_.size()), values_);
    return new llvm::GlobalVariable(*module, type_, true, llvm::GlobalValue::InternalLinkage, c);
  }

  std::string toString() const override
  {
    return "[ConstArray]";
  }

  llvm::Type *elementType()
  {
    return element_type_;
  }

  llvm::ArrayType *type()
  {
    return type_;
  }

protected:
  ConstantArray(TypeDeclaration const &type_decl, llvm::IRBuilder<> &builder,
                llvm::Type *element_type, ValueList const &values)
    : TypedValuePrototype(type_decl, builder)
    , element_type_{element_type}
    , values_{values}
    , type_{llvm::ArrayType::get(element_type_, values_.size())}
  {}

  llvm::Type      *element_type_{nullptr};
  ValueList        values_{};
  llvm::ArrayType *type_{nullptr};
};
using ConstantArrayPtr = ValueContianer<ConstantArray>;

}  // namespace compiler