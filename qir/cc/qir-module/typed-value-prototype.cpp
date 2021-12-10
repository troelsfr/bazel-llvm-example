#include "qir/cc/qir-module/typed-value-prototype.hpp"

#include "qir/cc/llvm/llvm.hpp"

#include <type_traits>
#include <typeindex>
namespace compiler {

TypedValuePrototype::TypedValuePrototype(std::type_index type_id, llvm::IRBuilder<> &builder)
  : type_id_{type_id}
  , builder_{builder}
{}

TypedValuePrototype::Constant *TypedValuePrototype::toConstant(LLVMContext *context,
                                                               Builder     &builder) const
{
  throw std::runtime_error("Value is not constant");
}

bool TypedValuePrototype::isLhs() const
{
  return false;
}

std::type_index TypedValuePrototype::typeId() const
{
  return type_id_;
}

TypedValuePrototype::Value *TypedValuePrototype::readValue()
{
  if (!value_)
  {
    auto &context = builder_.getContext();
    value_        = toValue(&context, builder_);
  }

  return value_;
}

TypedValuePrototype::Constant *TypedValuePrototype::readConstant()
{
  if (!constant_)
  {
    auto &context = builder_.getContext();
    constant_     = toConstant(&context, builder_);
    value_        = constant_;
  }

  return constant_;
}

void TypedValuePrototype::writeValue(Value *)
{
  throw std::runtime_error("variable or expression is read only");
}

TypedValuePrototypePtr TypedValuePrototype::getArrayElement(TypedValuePrototypePtr const &index)
{
  throw std::runtime_error("Expression or identifier is not subscriptable");
}

}  // namespace compiler