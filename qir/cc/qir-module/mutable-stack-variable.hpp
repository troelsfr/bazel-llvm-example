#pragma once
#include "qir/cc/llvm/llvm.hpp"
#include "qir/cc/qir-module/qir-program.hpp"

#include <typeindex>
#include <unordered_map>
#include <vector>
namespace compiler {

class MutableStackVariable : public TypedValuePrototype
{
public:
  using MutableStackVariablePtr = ValueContianer<MutableStackVariable>;

  static MutableStackVariablePtr create(QirType type, llvm::IRBuilder<> &builder,
                                        llvm::AllocaInst *instr, QirProgram &qir_program)
  {
    MutableStackVariablePtr ret;
    ret.reset(new MutableStackVariable(type, builder, instr, qir_program));
    return ret;
  }

  Value *toValue(LLVMContext * /*context*/, Builder & /*builder*/) const override
  {

    return instr_;
  }

  std::string toString() const override
  {
    return "MutableStackVariable";
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
  MutableStackVariable(QirType type, Builder &builder, llvm::AllocaInst *instr,
                       QirProgram &qir_program)
    : TypedValuePrototype(type.type_id, builder)
    , type_{type}
    , instr_{instr}
    , qir_program_{qir_program}
    , builder_{builder}
  {}

  QirType            type_;
  llvm::AllocaInst  *instr_;
  QirProgram        &qir_program_;
  llvm::IRBuilder<> &builder_;
};
using MutableStackVariablePtr = ValueContianer<MutableStackVariable>;

}  // namespace compiler