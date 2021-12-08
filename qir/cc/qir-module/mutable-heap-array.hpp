#pragma once
#include "qir/cc/llvm/llvm.hpp"
#include "qir/cc/qir-module/qir-program.hpp"

#include <typeindex>
#include <unordered_map>
#include <vector>
namespace compiler {

class MutableHeapArray : public TypedValuePrototype
{
public:
  using MutableHeapArrayPtr = ValueContianer<MutableHeapArray>;

  static MutableHeapArrayPtr create(QirType type, Builder &builder, llvm::Instruction *instr,
                                    QirProgram &qir_program)
  {
    MutableHeapArrayPtr ret;
    ret.reset(new MutableHeapArray(type, builder, instr, qir_program));
    return ret;
  }

  Value *toValue(LLVMContext * /*context*/, Builder & /*builder*/) const override
  {

    return instr_;
  }

  std::string toString() const override
  {
    return "MutableHeapArray";
  }

  void set(TypedValuePrototypePtr const &index, TypedValuePrototypePtr const &value)
  {
    auto idx = index->toValue(qir_program_.context(), builder_);
    auto val = value->toValue(qir_program_.context(), builder_);
    auto ptr = builder_.CreateGEP(type_.value, instr_, idx);
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
  MutableHeapArray(QirType type, Builder &builder, llvm::Instruction *instr,
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
using MutableHeapArrayPtr = ValueContianer<MutableHeapArray>;

}  // namespace compiler