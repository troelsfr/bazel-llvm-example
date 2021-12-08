#pragma once
#include "qir/cc/llvm/llvm.hpp"
#include "qir/cc/qir-module/value-container.hpp"

#include <type_traits>
#include <typeindex>
namespace compiler {

class TypedValue;
class TypedValuePrototype
{
public:
  using Value       = llvm::Value;
  using Constant    = llvm::Constant;
  using LLVMContext = llvm::LLVMContext;
  using Builder     = llvm::IRBuilder<>;

  using Assigned = void;

  TypedValuePrototype(std::type_index type_id, llvm::IRBuilder<> &builder);
  virtual ~TypedValuePrototype() = default;

  virtual Constant *toConstant(LLVMContext *context, Builder &builder) const;

  virtual Value      *toValue(LLVMContext *context, Builder &builder) const = 0;
  virtual std::string toString() const                                      = 0;
  virtual bool        isLhs() const;

  std::type_index typeId() const;

  llvm::IRBuilder<> &builder() const
  {
    return builder_;
  }

protected:
  std::type_index    type_id_;
  llvm::IRBuilder<> &builder_;
};
using TypedValuePrototypePtr = ValueContianer<TypedValuePrototype>;

}  // namespace compiler