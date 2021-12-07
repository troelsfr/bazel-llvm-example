#pragma once
#include "qir/cc/llvm/llvm.hpp"
#include "qir/cc/qir-module/qir-program.hpp"

#include <typeindex>
#include <unordered_map>
#include <vector>
namespace compiler {

class MutableStackArray : public TypedValuePrototype
{
public:
  using MutableStackArrayPtr = ValueContianer<MutableStackArray>;

  static MutableStackArrayPtr create(QirType type, Builder &builder, llvm::AllocaInst *instr,
                                     QirProgram &qir_program)
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
    auto val = value->toValue(qir_program_.context(), builder_);
    auto ptr =
        builder_.CreateGEP(type_.value, instr_, index->toValue(qir_program_.context(), builder_));
    TypedValue::create(type_.type_id, builder_, builder_.CreateStore(val, ptr));
  }

  TypedValuePtr get(TypedValuePrototypePtr const &index)
  {
    auto ptr =
        builder_.CreateGEP(type_.value, instr_, index->toValue(qir_program_.context(), builder_));
    auto ret = TypedValue::create(type_.type_id, builder_, builder_.CreateLoad(type_.value, ptr));
    return ret;
  }

private:
  MutableStackArray(QirType type, Builder &builder, llvm::AllocaInst *instr,
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
using MutableStackArrayPtr = ValueContianer<MutableStackArray>;

}  // namespace compiler