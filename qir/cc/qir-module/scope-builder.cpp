// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "qir/cc/qir-module/scope-builder.hpp"

#include "qir/cc/qir-module/constant-array.hpp"
#include "qir/cc/qir-module/constant-int.hpp"

#include <iostream>
#include <vector>

extern "C" void log_message(const char *s);
namespace compiler {

ScopeBuilderPtr ScopeBuilder::create(ScriptBuilder &qir_program, ScopeRegisterPtr const &scope,
                                     LlvmBlock *block)
{
  ScopeBuilderPtr ret;
  ret.reset(new ScopeBuilder(qir_program, scope, block));
  return ret;
}

ScopeBuilder::ScopeBuilder(ScriptBuilder &qir_program, ScopeRegisterPtr const &scope,
                           LlvmBlock *block)
  : qir_program_{qir_program}
  , builder_{*qir_program.context()}
  , block_{block}
  , scope_{scope}
{
  qir_program_.addBuilder(this);
  builder_.SetInsertPoint(block_);
}

ScopeBuilder::~ScopeBuilder()
{
  finalise();
}

TypedValuePtr ScopeBuilder::call(FunctionDeclaration const &fnc, ValueList const &args)
{
  std::vector<llvm::Value *> llvm_args{};
  for (auto &a : args)
  {
    llvm_args.push_back(a->readValue());
  }
  auto llvm_value  = builder_.CreateCall(fnc.function, llvm_args);
  auto return_type = qir_program_.getType(fnc.return_type);
  return TypedValue::create(return_type, builder_, llvm_value);
}

void ScopeBuilder::returnValue(TypedValuePrototypePtr const &value)
{
  builder_.CreateRet(value->readValue());
}

IfStatementPtr ScopeBuilder::ifStatement(TypedValuePrototypePtr const &value)
{
  auto val = value->readValue();
  auto false_block =
      llvm::BasicBlock::Create(*qir_program_.context(), "exit_if", block_->getParent());
  auto true_block = llvm::BasicBlock::Create(*qir_program_.context(), "if_true",
                                             block_->getParent(), false_block);
  builder_.CreateCondBr(val, true_block, false_block);

  block_ = false_block;
  builder_.SetInsertPoint(block_);
  return IfStatement::create(qir_program_, scope_->childScope(), true_block, false_block);
}

ConstantIntegerPtr ScopeBuilder::toInt8(int8_t const &value)
{
  return ConstantInteger::createNew<int8_t>(qir_program_.getType(std::type_index(typeid(int8_t))),
                                            builder_, static_cast<uint64_t>(value));
}

ConstantIntegerPtr ScopeBuilder::toInt16(int16_t const &value)
{
  return ConstantInteger::createNew<int16_t>(qir_program_.getType(std::type_index(typeid(int16_t))),
                                             builder_, static_cast<uint64_t>(value));
}

ConstantIntegerPtr ScopeBuilder::toInt32(int32_t const &value)
{
  return ConstantInteger::createNew<int32_t>(qir_program_.getType(std::type_index(typeid(int32_t))),
                                             builder_, static_cast<uint64_t>(value));
}

ConstantIntegerPtr ScopeBuilder::toInt64(int64_t const &value)
{
  return ConstantInteger::createNew<int64_t>(qir_program_.getType(std::type_index(typeid(int64_t))),
                                             builder_, static_cast<uint64_t>(value));
}

ConstantArrayPtr ScopeBuilder::constantArray(TypeDeclaration const &element_type,
                                             ValueList const       &values)
{
  std::vector<llvm::Constant *> elements;
  for (auto &v : values)
  {
    elements.push_back(v->toConstant(qir_program_.context(), builder_));
  }

  return ConstantArray::createNew(element_type, builder_, element_type.llvm_type, elements);
}

MutableStackVariablePtr ScopeBuilder::newStackVariable(TypeDeclaration const &element_type,
                                                       String const          &name)
{
  auto instr = builder_.CreateAlloca(element_type.llvm_type, nullptr, name);
  return MutableStackVariable::create(element_type, builder_, instr, qir_program_);
}

MutableStackArrayPtr ScopeBuilder::newStackArray(TypeDeclaration const &element_type,
                                                 TypedValuePrototypePtr size, String const &name)
{
  // Stack store
  // https://llvm.org/doxygen/InlineFunction_8cpp_source.html#2251
  auto instr = builder_.CreateAlloca(element_type.llvm_type, size->readValue(), name);
  return MutableStackArray::create(element_type, builder_, instr, qir_program_);
}

MutableHeapVariablePtr ScopeBuilder::newHeapVariable(TypeDeclaration const &element_type,
                                                     String const          &name)
{
  Type *malloc_arg_type = llvm::Type::getInt64Ty(*qir_program_.context());
  auto  type_size       = toInt64(element_type.size)->toConstant(qir_program_.context(), builder_);

  auto alloc_type_size = llvm::ConstantExpr::getTruncOrBitCast(type_size, malloc_arg_type);
  auto instr = llvm::CallInst::CreateMalloc(block_, malloc_arg_type, element_type.llvm_type,
                                            alloc_type_size, nullptr, nullptr);

  builder_.Insert(instr);
  instr->setName(name);
  auto ret = MutableHeapVariable::create(element_type, builder_, instr, qir_program_);
  return ret;
}

MutableHeapArrayPtr ScopeBuilder::newHeapArray(TypeDeclaration const &element_type,
                                               TypedValuePrototypePtr size, String const &name)
{
  Type *malloc_arg_type = llvm::Type::getInt64Ty(*qir_program_.context());
  auto  type_size       = toInt64(element_type.size)->toConstant(qir_program_.context(), builder_);
  auto  llvm_size       = size->readValue();

  auto alloc_type_size = llvm::ConstantExpr::getTruncOrBitCast(type_size, malloc_arg_type);
  auto instr = llvm::CallInst::CreateMalloc(block_, malloc_arg_type, element_type.llvm_type,
                                            alloc_type_size, llvm_size, nullptr);

  builder_.Insert(instr);
  instr->setName(name);
  auto ret = MutableHeapArray::create(element_type, builder_, instr, qir_program_);
  return ret;
}

TypedValuePtr ScopeBuilder::constantGetElement(ConstantArrayPtr const   &array,
                                               ConstantIntegerPtr const &index)
{
  auto ptr_type = llvm::PointerType::get(array->elementType(), 0);

  auto llvm_arr   = array->toConstant(qir_program_.context(), builder_);
  auto llvm_index = index->toConstant(qir_program_.context(), builder_);

  auto ptr = llvm::ConstantExpr::getGetElementPtr(array->type(), llvm_arr, llvm_index);

  // TODO:
  auto raw_ptr = llvm::ConstantExpr::getBitCast(ptr, ptr_type);
  auto ret     = TypedValue::create(array->typeDeclaration(), builder_,
                                    builder_.CreateLoad(array->elementType(), raw_ptr));

  return ret;
}

bool ScopeBuilder::isActive()
{
  return block_->getTerminator() == nullptr;
}

void ScopeBuilder::requireIsActive()
{
  if (!isActive())
  {
    throw std::runtime_error("Block was finalised and cannot be modified.");
  }
}

Qubit ScopeBuilder::allocateQubit()
{
  requireIsActive();

  Qubit ret = {qubit_counter_};

  auto val        = builder_.CreateCall(qubit_allocator_, {});
  qubits_[ret.id] = val;

  ++qubit_counter_;
  return ret;
}

void ScopeBuilder::x(Qubit const &q)
{
  requireIsActive();

  auto arg = qubits_[q.id];
  builder_.CreateCall(quantum_x_, {arg});
}

void ScopeBuilder::z(Qubit const &q)
{
  requireIsActive();

  auto arg = qubits_[q.id];
  builder_.CreateCall(quantum_z_, {arg});
}

void ScopeBuilder::cnot(Qubit const &q1, Qubit const &q2)
{
  requireIsActive();

  auto arg1 = qubits_[q1.id];
  auto arg2 = qubits_[q2.id];

  builder_.CreateCall(quantum_cnot_, {arg1, arg2});
}

llvm::IRBuilder<> &ScopeBuilder::builder()
{
  return builder_;
}

void ScopeBuilder::finalise()
{
  // Note that we need finalise_called_ as isActive may become true
  // during tear down as a result of the terminator being deleted.
  // We also use this variable too ensure that the builder is removed
  // from the program exactly once.
  if (!finalise_called_)
  {
    qir_program_.removeBuilder(this);

    if (isActive())
    {
      if (add_terminator_)
      {
        add_terminator_(builder_);
        assert(!isActive());
      }
      else
      {
        builder_.CreateRetVoid();
        assert(!isActive());
      }
    }
  }

  finalise_called_ = true;
}

void ScopeBuilder::setTerminatorFunction(TerminatorFunction const &f)
{
  add_terminator_ = f;
}

}  // namespace compiler
