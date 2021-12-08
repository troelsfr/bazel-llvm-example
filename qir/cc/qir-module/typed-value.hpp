#pragma once
#include "qir/cc/llvm/llvm.hpp"
#include "qir/cc/qir-module/typed-value-prototype.hpp"
#include "qir/cc/qir-module/value-container.hpp"

#include <type_traits>
#include <typeindex>
namespace compiler {

class TypedValue : public TypedValuePrototype
{
public:
  using TypedValuePtr = ValueContianer<TypedValue>;

  static TypedValuePtr create(std::type_index type_id, llvm::IRBuilder<> &builder,
                              llvm::Value *value);
  Value               *toValue(LLVMContext *context, Builder &builder) const override;
  std::string          toString() const override;

private:
  TypedValue(std::type_index type_id, llvm::IRBuilder<> &builder, llvm::Value *value);

  llvm::Value *value_;
};
using TypedValuePtr = ValueContianer<TypedValue>;

inline TypedValuePtr operator+(TypedValuePrototypePtr const &a, TypedValuePrototypePtr const &b)
{
  if (a->typeId() != b->typeId())
  {
    throw std::runtime_error("Cannot add operands of different types.");
  }

  auto &context = a->builder().getContext();
  auto  a_val   = a->toValue(&context, a->builder());
  auto  b_val   = b->toValue(&context, b->builder());
  return TypedValue::create(a->typeId(), a->builder(), a->builder().CreateAdd(a_val, b_val));
}

inline TypedValuePtr operator*(TypedValuePrototypePtr const &a, TypedValuePrototypePtr const &b)
{
  if (a->typeId() != b->typeId())
  {
    throw std::runtime_error("Cannot multiply operands of different types.");
  }

  auto &context = a->builder().getContext();
  auto  a_val   = a->toValue(&context, a->builder());
  auto  b_val   = b->toValue(&context, b->builder());

  return TypedValue::create(a->typeId(), a->builder(), a->builder().CreateMul(a_val, b_val));
}

inline TypedValuePtr operator==(TypedValuePrototypePtr const &a, TypedValuePrototypePtr const &b)
{
  if (a->typeId() != b->typeId())
  {
    throw std::runtime_error("Cannot multiply operands of different types.");
  }

  auto &context = a->builder().getContext();
  auto  a_val   = a->toValue(&context, a->builder());
  auto  b_val   = b->toValue(&context, b->builder());

  return TypedValue::create(a->typeId(), a->builder(), a->builder().CreateICmpEQ(a_val, b_val));
}
}  // namespace compiler