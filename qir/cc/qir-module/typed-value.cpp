#include "qir/cc/qir-module/typed-value.hpp"

#include "qir/cc/llvm/llvm.hpp"
#include "qir/cc/qir-module/value-container.hpp"

#include <type_traits>
#include <typeindex>
namespace compiler {

TypedValuePtr TypedValue::create(std::type_index type_id, llvm::IRBuilder<> &builder,
                                 llvm::Value *value)
{
  TypedValuePtr ret;
  ret.reset(new TypedValue(type_id, builder, value));
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

TypedValue::TypedValue(std::type_index type_id, llvm::IRBuilder<> &builder, llvm::Value *value)
  : TypedValuePrototype(type_id, builder)
  , value_{value}
{}

}  // namespace compiler