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

}  // namespace compiler