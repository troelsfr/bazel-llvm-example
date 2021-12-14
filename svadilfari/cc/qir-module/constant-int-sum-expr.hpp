#pragma once
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "svadilfari/cc/llvm/llvm.hpp"
#include "svadilfari/cc/qir-module/constant-int.hpp"
#include "svadilfari/cc/qir-module/typed-value-prototype.hpp"

#include <type_traits>
#include <typeindex>

namespace compiler {

class IntegerSumExpr : public ConstantInteger
{
public:
  static ValueContianer<IntegerSumExpr> createNew(ConstantIntegerPtr lhs, ConstantIntegerPtr rhs)
  {
    ValueContianer<IntegerSumExpr> ret;
    ret.reset(new IntegerSumExpr(lhs, rhs));
    return ret;
  }

  Value *toValue(LLVMContext *context, Builder &builder) const override
  {
    auto a = lhs_->readValue();
    auto b = rhs_->readValue();

    return builder.CreateAdd(a, b);
  }

  std::string toString() const override
  {
    return lhs_->toString() + " + " + rhs_->toString();
  }

protected:
  IntegerSumExpr(ConstantIntegerPtr lhs, ConstantIntegerPtr rhs)
    : ConstantInteger(lhs->typeId(), lhs->bits())
    , lhs_{lhs}
    , rhs_{rhs}

  {}

  ConstantIntegerPtr lhs_;
  ConstantIntegerPtr rhs_;
};
using IntegerSumExprPtr = ValueContianer<IntegerSumExpr>;

}  // namespace compiler