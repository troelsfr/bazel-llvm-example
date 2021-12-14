#pragma once
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "svadilfari/cc/llvm/llvm.hpp"
#include "svadilfari/cc/qir-module/script-builder.hpp"

#include <typeindex>
#include <unordered_map>
#include <vector>
namespace compiler {

class MutableHeapArray : public TypedValuePrototype
{
public:
  using MutableHeapArrayPtr = ValueContianer<MutableHeapArray>;

  static MutableHeapArrayPtr create(TypeDeclaration type, Builder &builder,
                                    llvm::Instruction *instr, ScriptBuilder &qir_program)
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
    auto idx = index->readValue();
    auto val = value->readValue();
    auto ptr = builder_.CreateGEP(type_decl_.llvm_type, instr_, idx);
    TypedValue::create(type_decl_, builder_, builder_.CreateStore(val, ptr));
  }

  TypedValuePtr get(TypedValuePrototypePtr const &index)
  {
    auto ptr = builder_.CreateGEP(type_decl_.llvm_type, instr_, index->readValue());
    auto ret =
        TypedValue::create(type_decl_, builder_, builder_.CreateLoad(type_decl_.llvm_type, ptr));
    return ret;
  }

private:
  MutableHeapArray(TypeDeclaration type, Builder &builder, llvm::Instruction *instr,
                   ScriptBuilder &qir_program)
    : TypedValuePrototype(type, builder)
    , instr_{instr}
    , builder_{builder}
  {}

  llvm::Instruction *instr_;
  llvm::IRBuilder<> &builder_;
};
using MutableHeapArrayPtr = ValueContianer<MutableHeapArray>;

}  // namespace compiler