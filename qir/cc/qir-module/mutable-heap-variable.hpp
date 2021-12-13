#pragma once
#include "qir/cc/llvm/llvm.hpp"
#include "qir/cc/qir-module/script-builder.hpp"

#include <typeindex>
#include <unordered_map>
#include <vector>
namespace compiler {

class MutableHeapVariable : public TypedValuePrototype
{
public:
  using MutableHeapVariablePtr = ValueContianer<MutableHeapVariable>;

  static MutableHeapVariablePtr create(TypeDeclaration const &type, llvm::IRBuilder<> &builder,
                                       llvm::Instruction *instr, ScriptBuilder &qir_program)
  {
    MutableHeapVariablePtr ret;
    ret.reset(new MutableHeapVariable(type, builder, instr, qir_program));
    return ret;
  }

  Value *toValue(LLVMContext * /*context*/, Builder & /*builder*/) const override
  {
    return instr_;
  }

  std::string toString() const override
  {
    return "MutableHeapVariable";
  }

  /// TODO: Deprecated
  void set(TypedValuePrototypePtr const &value)
  {
    builder_.CreateStore(value->readValue(), instr_);
  }

  TypedValuePtr get()
  {
    auto ret =
        TypedValue::create(type_decl_, builder_, builder_.CreateLoad(type_decl_.llvm_type, instr_));
    return ret;
  }
  /// TODO: End deprecated

  void writeValue(Value *val) override
  {
    builder_.CreateStore(val, instr_);
  }

  Value *readValue() override
  {
    return builder_.CreateLoad(type_decl_.llvm_type, instr_);
  }

private:
  MutableHeapVariable(TypeDeclaration const &type, Builder &builder, llvm::Instruction *instr,
                      ScriptBuilder &qir_program)
    : TypedValuePrototype(type, builder)
    , instr_{instr}
    , builder_{builder}
  {}

  //  TypeDeclaration    type_decl_;
  llvm::Instruction *instr_;
  llvm::IRBuilder<> &builder_;
};
using MutableHeapVariablePtr = ValueContianer<MutableHeapVariable>;

}  // namespace compiler