#pragma once
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "svadilfari/cc/llvm/llvm.hpp"
#include "svadilfari/cc/script-builder/typed-value-prototype.hpp"

#include <type_traits>
#include <typeindex>

namespace compiler {
namespace details {
template <typename T>
struct BitCounter
{
};

template <>
struct BitCounter<int8_t>
{
  constexpr static uint32_t value()
  {
    return 8;
  }
};

template <>
struct BitCounter<int16_t>
{
  constexpr static uint32_t value()
  {
    return 16;
  }
};

template <>
struct BitCounter<int32_t>
{
  constexpr static uint32_t value()
  {
    return 32;
  }
};

template <>
struct BitCounter<int64_t>
{
  constexpr static uint32_t value()
  {
    return 64;
  }
};

}  // namespace details

class ConstantInteger : public TypedValuePrototype
{
public:
  template <typename T>
  static ValueContianer<ConstantInteger> createNew(TypeDeclaration const &type_decl,
                                                   llvm::IRBuilder<> &builder, T value = 0)
  {
    ValueContianer<ConstantInteger> ret;
    ret.reset(new ConstantInteger(type_decl, builder, details::BitCounter<T>::value(), value));
    return ret;
  }

  static ValueContianer<ConstantInteger> createNew(TypeDeclaration const &type_decl,
                                                   llvm::IRBuilder<> &builder, uint32_t bits,
                                                   uint64_t value = 0)
  {
    ValueContianer<ConstantInteger> ret;
    ret.reset(new ConstantInteger(type_decl, builder, bits, value));
    return ret;
  }
  virtual ~ConstantInteger() = default;

  Constant *toConstant(LLVMContext *context, Builder & /*builder*/) const override
  {
    auto ret = llvm::ConstantInt::get(*context, llvm::APInt(bits_, static_cast<uint64_t>(value_)));
    // builder.Insert(ret);
    return ret;
  }

  Value *toValue(LLVMContext *context, Builder & /*builder*/) const override
  {
    auto ret = llvm::ConstantInt::get(*context, llvm::APInt(bits_, static_cast<uint64_t>(value_)));
    // builder.Insert(ret);
    return ret;
  }

  std::string toString() const override
  {
    return std::to_string(value_) + "i" + std::to_string(bits_);
  }

  uint32_t bits() const
  {
    return bits_;
  }

protected:
  ConstantInteger(TypeDeclaration const &type_decl, llvm::IRBuilder<> &builder, uint32_t bits,
                  uint64_t value = 0)
    : TypedValuePrototype(type_decl, builder)
    , bits_{bits}
    , value_{value}
  {}

  uint32_t bits_;
  uint64_t value_;
};
using ConstantIntegerPtr = ValueContianer<ConstantInteger>;

/*
class SumExpr : public ConstantInteger
{
public:
  static ValueContianer<SumExpr> createNew(ConstantIntegerPtr lhs, ConstantIntegerPtr rhs)
  {
    ValueContianer<SumExpr> ret;
    ret.reset(new SumExpr(lhs, rhs));
    return ret;
  }

  Value *toValue(LLVMContext *context, Builder &builder) const override
  {
    auto a = lhs_->toValue(context, builder);
    auto b = rhs_->toValue(context, builder);

    return builder.CreateAdd(a, b);
  }

  std::string toString() const override
  {
    return lhs_->toString() + " + " + rhs_->toString();
  }

protected:
  SumExpr(ConstantIntegerPtr lhs, ConstantIntegerPtr rhs)
    : ConstantInteger(lhs->typeId(), lhs->bits())
    , lhs_{lhs}
    , rhs_{rhs}

  {}

  ConstantIntegerPtr lhs_;
  ConstantIntegerPtr rhs_;
};
using SumExprPtr = ValueContianer<SumExpr>;
*/
}  // namespace compiler