#pragma once
#include "qir/cc/llvm/llvm.hpp"

#include <iostream>
#include <unordered_map>
#include <vector>

struct Qubit
{
  uint64_t id;
};

class QirBuilder
{
public:
  using Type         = llvm::Type;
  using StructType   = llvm::StructType;
  using LlvmContext  = llvm::LLVMContext;
  using LlvmModule   = llvm::Module;
  using Function     = llvm::Function;
  using Constant     = llvm::Constant;
  using ConstantInt  = llvm::ConstantInt;
  using String       = std::string;
  using LlvmFunction = llvm::Function;

  QirBuilder(String const &main_function);
  String getQir();

  Qubit allocateQubit();
  void  x(Qubit const &q);
  void  z(Qubit const &q);
  void  cnot(Qubit const &q1, Qubit const &q2);

private:
  std::unique_ptr<LlvmContext> context_{};
  std::unique_ptr<LlvmModule>  module_{};
  llvm::IRBuilder<>            builder_;
  LlvmFunction *               function_{nullptr};
  LlvmFunction *               qubit_allocator_{nullptr};
  LlvmFunction *               quantum_x_{nullptr};
  LlvmFunction *               quantum_z_{nullptr};
  LlvmFunction *               quantum_cnot_{nullptr};

  std::unordered_map<uint64_t, llvm::Value *> qubits_;
  uint64_t                                    qubit_counter_{0};

  llvm::Type *qubit_type_;
};