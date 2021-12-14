#pragma once
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "svadilfari/cc/llvm/llvm.hpp"
#include "svadilfari/cc/script-builder/mutable-stack-variable.hpp"
#include "svadilfari/cc/script-builder/script-builder.hpp"

#include <typeindex>
#include <unordered_map>
#include <vector>
namespace compiler {

class MutableStackArray : public TypedValuePrototype
{
public:
  using MutableStackArrayPtr = ValueContianer<MutableStackArray>;

  static MutableStackArrayPtr create(TypeDeclaration type, Builder &builder,
                                     llvm::AllocaInst *instr, ScriptBuilder &script_builder)
  {
    MutableStackArrayPtr ret;
    ret.reset(new MutableStackArray(type, builder, instr, script_builder));
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
    auto ptr = builder_.CreateGEP(type_decl_.llvm_type, instr_, index->readValue());
    TypedValue::create(type_decl_, builder_, builder_.CreateStore(val, ptr));
  }

  TypedValuePtr get(TypedValuePrototypePtr const &index)
  {
    auto ptr = builder_.CreateGEP(type_decl_.llvm_type, instr_, index->readValue());
    auto ret =
        TypedValue::create(type_decl_, builder_, builder_.CreateLoad(type_decl_.llvm_type, ptr));
    return ret;
  }

  TypedValuePrototypePtr getArrayElement(TypedValuePrototypePtr const &index) override
  {
    auto size   = script_builder_.toInt64(builder_, type_decl_.size);
    auto offset = builder_.CreateMul(size->readValue(), index->readValue());
    auto ptr    = builder_.CreateGEP(type_decl_.llvm_type, instr_, offset);

    return MutableStackVariable::create(type_decl_, builder_, ptr, script_builder_);
  }

private:
  MutableStackArray(TypeDeclaration type, Builder &builder, llvm::AllocaInst *instr,
                    ScriptBuilder &script_builder)
    : TypedValuePrototype(type, builder)
    , instr_{instr}
    , builder_{builder}
    , script_builder_{script_builder}
  {}

  llvm::AllocaInst  *instr_;
  llvm::IRBuilder<> &builder_;
  ScriptBuilder     &script_builder_;
};
using MutableStackArrayPtr = ValueContianer<MutableStackArray>;

}  // namespace compiler