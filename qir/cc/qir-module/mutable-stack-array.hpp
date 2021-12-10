#pragma once
#include "qir/cc/llvm/llvm.hpp"
#include "qir/cc/qir-module/mutable-stack-variable.hpp"
#include "qir/cc/qir-module/script-builder.hpp"

#include <typeindex>
#include <unordered_map>
#include <vector>
namespace compiler {

class MutableStackArray : public TypedValuePrototype
{
public:
  using MutableStackArrayPtr = ValueContianer<MutableStackArray>;

  static MutableStackArrayPtr create(QirType type, Builder &builder, llvm::AllocaInst *instr,
                                     ScriptBuilder &qir_program)
  {
    MutableStackArrayPtr ret;
    ret.reset(new MutableStackArray(type, builder, instr, qir_program));
    return ret;
  }

  Value *toValue(LLVMContext * /*context*/, Builder & /*builder*/) const override
  {

    return instr_;
  }

  std::string toString() const override
  {
    return "MutableStackArray";
  }

  void set(TypedValuePrototypePtr const &index, TypedValuePrototypePtr const &value)
  {
    auto val = value->readValue();
    auto ptr = builder_.CreateGEP(type_.value, instr_, index->readValue());
    TypedValue::create(type_.type_id, builder_, builder_.CreateStore(val, ptr));
  }

  TypedValuePtr get(TypedValuePrototypePtr const &index)
  {
    auto ptr = builder_.CreateGEP(type_.value, instr_, index->readValue());
    auto ret = TypedValue::create(type_.type_id, builder_, builder_.CreateLoad(type_.value, ptr));
    return ret;
  }

  TypedValuePrototypePtr getArrayElement(TypedValuePrototypePtr const &index) override
  {
    auto size   = qir_program_.toInt64(builder_, type_.size);
    auto offset = builder_.CreateMul(size->readValue(), index->readValue());
    auto ptr    = builder_.CreateGEP(type_.value, instr_, offset);

    return MutableStackVariable::create(type_, builder_, ptr, qir_program_);
  }

private:
  MutableStackArray(QirType type, Builder &builder, llvm::AllocaInst *instr,
                    ScriptBuilder &qir_program)
    : TypedValuePrototype(type.type_id, builder)
    , type_{type}
    , instr_{instr}
    , builder_{builder}
    , qir_program_{qir_program}
  {}

  QirType            type_;
  llvm::AllocaInst  *instr_;
  llvm::IRBuilder<> &builder_;
  ScriptBuilder     &qir_program_;
};
using MutableStackArrayPtr = ValueContianer<MutableStackArray>;

}  // namespace compiler