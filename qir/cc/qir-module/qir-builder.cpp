#include "qir/cc/qir-module/qir-builder.hpp"

#include "qir/cc/qir-module/constant-array.hpp"
#include "qir/cc/qir-module/constant-int.hpp"

#include <iostream>
#include <vector>

extern "C" void log_message(const char *s);
namespace compiler {

QirBuilderPtr QirBuilder::create(QirProgram &qir_program, LlvmBlock *block)
{
  QirBuilderPtr ret;
  ret.reset(new QirBuilder(qir_program, block));
  return ret;
}

QirBuilder::QirBuilder(QirProgram &qir_program, LlvmBlock *block)
  : qir_program_{qir_program}
  , builder_{*qir_program.context()}
  , block_{block}

{
  setupBuiltIns();

  builder_.SetInsertPoint(block_);
}

void QirBuilder::returnValue(TypedValuePrototypePtr const &value)
{
  builder_.CreateRet(value->toValue(qir_program_.context(), builder_));
}

ConstantIntegerPtr QirBuilder::toInt8(int8_t const &value)
{
  return ConstantInteger::createNew<int8_t>(builder_, static_cast<uint64_t>(value));
}

ConstantIntegerPtr QirBuilder::toInt16(int16_t const &value)
{
  return ConstantInteger::createNew<int16_t>(builder_, static_cast<uint64_t>(value));
}

ConstantIntegerPtr QirBuilder::toInt32(int32_t const &value)
{
  return ConstantInteger::createNew<int32_t>(builder_, static_cast<uint64_t>(value));
}

ConstantIntegerPtr QirBuilder::toInt64(int64_t const &value)
{
  return ConstantInteger::createNew<int64_t>(builder_, static_cast<uint64_t>(value));
}

ConstantArrayPtr QirBuilder::constantArray(QirType element_type, ValueList const &values)
{
  std::vector<llvm::Constant *> elements;
  for (auto &v : values)
  {
    elements.push_back(v->toConstant(qir_program_.context(), builder_));
  }

  return ConstantArray::createNew(element_type.type_id, builder_, element_type.value, elements);
}

MutableStackVariablePtr QirBuilder::newStackVariable(QirType element_type, String const &name)
{
  auto instr = builder_.CreateAlloca(element_type.value, nullptr, name);
  return MutableStackVariable::create(element_type, builder_, instr, qir_program_);
}

MutableStackArrayPtr QirBuilder::newStackArray(QirType element_type, TypedValuePrototypePtr size,
                                               String const &name)
{
  // Stack store
  // https://llvm.org/doxygen/InlineFunction_8cpp_source.html#2251
  auto instr = builder_.CreateAlloca(element_type.value,
                                     size->toValue(qir_program_.context(), builder_), name);
  return MutableStackArray::create(element_type, builder_, instr, qir_program_);
}

MutableHeapVariablePtr QirBuilder::newHeapVariable(QirType element_type, String const &name)
{
  Type *malloc_arg_type = llvm::Type::getInt64Ty(*qir_program_.context());
  auto  type_size       = toInt64(element_type.size)->toConstant(qir_program_.context(), builder_);

  auto alloc_type_size = llvm::ConstantExpr::getTruncOrBitCast(type_size, malloc_arg_type);
  auto instr           = llvm::CallInst::CreateMalloc(block_, malloc_arg_type,
                                                      llvm::PointerType::get(element_type.value, 0),
                                                      alloc_type_size, nullptr, nullptr);

  builder_.Insert(instr);
  instr->setName(name);
  auto ret = MutableHeapVariable::create(element_type, builder_, instr, qir_program_);
  return ret;
}

MutableHeapArrayPtr QirBuilder::newHeapArray(QirType element_type, TypedValuePrototypePtr size,
                                             String const &name)
{
  Type *malloc_arg_type = llvm::Type::getInt64Ty(*qir_program_.context());
  auto  type_size       = toInt64(element_type.size)->toConstant(qir_program_.context(), builder_);
  auto  llvm_size       = size->toValue(qir_program_.context(), builder_);

  auto alloc_type_size = llvm::ConstantExpr::getTruncOrBitCast(type_size, malloc_arg_type);
  auto instr           = llvm::CallInst::CreateMalloc(block_, malloc_arg_type,
                                                      llvm::PointerType::get(element_type.value, 0),
                                                      alloc_type_size, llvm_size, nullptr);

  builder_.Insert(instr);
  instr->setName(name);
  auto ret = MutableHeapArray::create(element_type, builder_, instr, qir_program_);
  return ret;
}

ConstantIntegerPtr QirBuilder::constantGetElement(ConstantArrayPtr const   &array,
                                                  ConstantIntegerPtr const &index)
{
  llvm::errs() << *array->elementType() << "\n";
  auto llvm_arr   = array->toConstant(qir_program_.context(), builder_);
  auto llvm_index = index->toConstant(qir_program_.context(), builder_);
  llvm::errs() << *llvm_arr << "\n";
  llvm::errs() << *llvm_index << "\n";

  llvm::errs() << *llvm::ConstantExpr::getGetElementPtr(array->type(), llvm_arr, llvm_index)
               << "\n";
  return index;
}

void QirBuilder::setupBuiltIns()
{
  std::vector<Type *> no_args{};
  auto                qubit_type = qir_program_.getLlvmType("Qubit");
  auto                void_type  = qir_program_.getLlvmType("Void");
  // Qubit allocator
  llvm::FunctionType *qubit_alloc_signature = llvm::FunctionType::get(qubit_type, no_args, false);
  qubit_allocator_ =
      llvm::Function::Create(qubit_alloc_signature, llvm::Function::LinkageTypes::ExternalLinkage,
                             "__quantum__qis__qubit_create", qir_program_.module());

  /// X
  std::vector<Type *> arg_types1{qubit_type};
  llvm::FunctionType *quantum_x_signature = llvm::FunctionType::get(void_type, arg_types1, false);
  quantum_x_ =
      llvm::Function::Create(quantum_x_signature, llvm::Function::LinkageTypes::ExternalLinkage,
                             "__quantum__qis__x__body", qir_program_.module());

  /// Z
  llvm::FunctionType *quantum_z_signature = llvm::FunctionType::get(void_type, arg_types1, false);
  quantum_z_ =
      llvm::Function::Create(quantum_z_signature, llvm::Function::LinkageTypes::ExternalLinkage,
                             "__quantum__qis__z__body", qir_program_.module());

  /// CNOT
  std::vector<Type *> arg_types2{qubit_type, qubit_type};
  llvm::FunctionType *quantum_cnot_signature =
      llvm::FunctionType::get(void_type, arg_types2, false);
  quantum_cnot_ =
      llvm::Function::Create(quantum_cnot_signature, llvm::Function::LinkageTypes::ExternalLinkage,
                             "__quantum__qis__cnot__body", qir_program_.module());
}

void QirBuilder::requireIsActive()
{
  if (!is_active_)
  {
    throw std::runtime_error("Block was finalised and cannot be modified.");
  }
}

Qubit QirBuilder::allocateQubit()
{
  requireIsActive();

  Qubit ret = {qubit_counter_};

  auto val        = builder_.CreateCall(qubit_allocator_, {});
  qubits_[ret.id] = val;

  ++qubit_counter_;
  return ret;
}

void QirBuilder::x(Qubit const &q)
{
  requireIsActive();

  auto arg = qubits_[q.id];
  builder_.CreateCall(quantum_x_, {arg});
}

void QirBuilder::z(Qubit const &q)
{
  requireIsActive();

  auto arg = qubits_[q.id];
  builder_.CreateCall(quantum_z_, {arg});
}

void QirBuilder::cnot(Qubit const &q1, Qubit const &q2)
{
  requireIsActive();

  auto arg1 = qubits_[q1.id];
  auto arg2 = qubits_[q2.id];

  builder_.CreateCall(quantum_cnot_, {arg1, arg2});
}

}  // namespace compiler
