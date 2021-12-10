#pragma once
#include "qir/cc/llvm/llvm.hpp"
#include "qir/cc/qir-module/qir-program.hpp"

#include <typeindex>
#include <unordered_map>
#include <vector>
namespace compiler {

class MutableHeapVariable : public TypedValuePrototype
{
public:
  using MutableHeapVariablePtr = ValueContianer<MutableHeapVariable>;

  static MutableHeapVariablePtr create(QirType type, llvm::IRBuilder<> &builder,
                                       llvm::Instruction *instr, QirProgram &qir_program)
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
        TypedValue::create(type_.type_id, builder_, builder_.CreateLoad(type_.value, instr_));
    return ret;
  }
  /// TODO: End deprecated

  void writeValue(Value *val) override
  {
    builder_.CreateStore(val, instr_);
  }

  Value *readValue() override
  {
    return builder_.CreateLoad(type_.value, instr_);
  }

private:
  MutableHeapVariable(QirType type, Builder &builder, llvm::Instruction *instr,
                      QirProgram &qir_program)
    : TypedValuePrototype(type.type_id, builder)
    , type_{type}
    , instr_{instr}
    , builder_{builder}
  {}

  QirType            type_;
  llvm::Instruction *instr_;
  llvm::IRBuilder<> &builder_;
};
using MutableHeapVariablePtr = ValueContianer<MutableHeapVariable>;

}  // namespace compiler