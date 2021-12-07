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

  void set(TypedValuePrototypePtr const &value)
  {
    builder_.CreateStore(value->toValue(qir_program_.context(), builder_), instr_);
  }

  TypedValuePtr get()
  {
    auto ret =
        TypedValue::create(type_.type_id, builder_, builder_.CreateLoad(type_.value, instr_));
    return ret;
  }

private:
  MutableHeapVariable(QirType type, Builder &builder, llvm::Instruction *instr,
                      QirProgram &qir_program)
    : TypedValuePrototype(type.type_id, builder)
    , type_{type}
    , instr_{instr}
    , qir_program_{qir_program}
    , builder_{builder}
  {}

  QirType            type_;
  llvm::Instruction *instr_;
  QirProgram        &qir_program_;
  llvm::IRBuilder<> &builder_;
};
using MutableHeapVariablePtr = ValueContianer<MutableHeapVariable>;

}  // namespace compiler