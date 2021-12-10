#pragma once
#include "qir/cc/llvm/llvm.hpp"
#include "qir/cc/qir-module/script-builder.hpp"

#include <typeindex>
#include <unordered_map>
#include <vector>
namespace compiler {

class MutableStackVariable : public TypedValuePrototype
{
public:
  using MutableStackVariablePtr = ValueContianer<MutableStackVariable>;

  static MutableStackVariablePtr create(QirType type, llvm::IRBuilder<> &builder,
                                        llvm::Value *instr, ScriptBuilder &qir_program)
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

  // TODO: Deprecated
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
    read_cache_ = nullptr;
    builder_.CreateStore(val, instr_);
  }

  Value *readValue() override
  {
    if (read_cache_ == nullptr)
    {
      read_cache_ = builder_.CreateLoad(type_.value, instr_);
    }

    return read_cache_;
  }

private:
  MutableStackVariable(QirType type, Builder &builder, llvm::Value *instr,
                       ScriptBuilder &qir_program)
    : TypedValuePrototype(type.type_id, builder)
    , type_{type}
    , instr_{instr}
    , builder_{builder}
  {}

  QirType      type_;
  llvm::Value *instr_;

  llvm::IRBuilder<> &builder_;

  Value *read_cache_{nullptr};
};
using MutableStackVariablePtr = ValueContianer<MutableStackVariable>;

}  // namespace compiler