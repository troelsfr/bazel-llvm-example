#pragma once
#include "qir/cc/llvm/llvm.hpp"
#include "qir/cc/qir-module/constant-int.hpp"
#include "qir/cc/qir-module/typed-value-prototype.hpp"

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
    auto a = lhs_->toValue(context, builder);
    auto b = rhs_->toValue(context, builder);

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