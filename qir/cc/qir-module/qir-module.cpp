#include "qir/cc/qir-module/qir-module.hpp"

#include <iostream>
#include <vector>

extern "C" void log_message(const char *s);

QirBuilder::QirBuilder(String const &main_function)
  : context_{std::make_unique<LlvmContext>()}
  , module_(std::make_unique<LlvmModule>("qir.ll", *context_))
  , builder_{*context_}
  , qubit_type_{llvm::StructType::create(*context_, "Qubit")}
{
  // Creating main function that returns an int8
  std::vector<Type *> no_args{};
  auto                return_type        = llvm::Type::getInt8Ty(*context_);
  llvm::FunctionType *function_signature = llvm::FunctionType::get(return_type, no_args, false);

  function_ =
      llvm::Function::Create(function_signature, llvm::Function::LinkageTypes::ExternalLinkage,
                             main_function, module_.get());

  // Qubit allocator
  llvm::FunctionType *qubit_alloc_signature = llvm::FunctionType::get(qubit_type_, no_args, false);
  qubit_allocator_ =
      llvm::Function::Create(qubit_alloc_signature, llvm::Function::LinkageTypes::ExternalLinkage,
                             "__quantum__qis__qubit_create", module_.get());

  /// X
  std::vector<Type *> arg_types1{qubit_type_};
  llvm::FunctionType *quantum_x_signature = llvm::FunctionType::get(qubit_type_, arg_types1, false);
  quantum_x_ =
      llvm::Function::Create(quantum_x_signature, llvm::Function::LinkageTypes::ExternalLinkage,
                             "__quantum__qis__x__body", module_.get());

  /// Z
  llvm::FunctionType *quantum_z_signature = llvm::FunctionType::get(qubit_type_, arg_types1, false);
  quantum_z_ =
      llvm::Function::Create(quantum_z_signature, llvm::Function::LinkageTypes::ExternalLinkage,
                             "__quantum__qis__z__body", module_.get());

  /// CNOT
  std::vector<Type *> arg_types2{qubit_type_, qubit_type_};
  llvm::FunctionType *quantum_cnot_signature =
      llvm::FunctionType::get(qubit_type_, arg_types2, false);
  quantum_cnot_ =
      llvm::Function::Create(quantum_cnot_signature, llvm::Function::LinkageTypes::ExternalLinkage,
                             "__quantum__qis__cnot__body", module_.get());

  auto block = llvm::BasicBlock::Create(*context_, "entry", function_);
  builder_.SetInsertPoint(block);
}

Qubit QirBuilder::allocateQubit()
{
  log_message("Allocating qubit");
  Qubit ret = {qubit_counter_};

  auto val        = builder_.CreateCall(qubit_allocator_, {});
  qubits_[ret.id] = val;

  ++qubit_counter_;
  return ret;
}

void QirBuilder::x(Qubit const &q)
{
  log_message("Applying X");

  auto arg = qubits_[q.id];
  builder_.CreateCall(quantum_x_, {arg});
}

void QirBuilder::z(Qubit const &q)
{
  log_message("Applying Z");

  auto arg = qubits_[q.id];
  builder_.CreateCall(quantum_z_, {arg});
}

void QirBuilder::cnot(Qubit const &q1, Qubit const &q2)
{
  log_message("Applying CNOT");

  auto arg1 = qubits_[q1.id];
  auto arg2 = qubits_[q2.id];

  builder_.CreateCall(quantum_cnot_, {arg1, arg2});
}

QirBuilder::String QirBuilder::getQir()
{
  String                   str;
  llvm::raw_string_ostream ostream(str);
  ostream << *module_;
  ostream.flush();
  return str;
}
