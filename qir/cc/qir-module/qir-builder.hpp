#pragma once
#include "qir/cc/llvm/llvm.hpp"
#include "qir/cc/qir-module/constant-array.hpp"
#include "qir/cc/qir-module/constant-int.hpp"
#include "qir/cc/qir-module/mutable-heap-array.hpp"
#include "qir/cc/qir-module/mutable-heap-variable.hpp"
#include "qir/cc/qir-module/mutable-stack-array.hpp"
#include "qir/cc/qir-module/mutable-stack-variable.hpp"
#include "qir/cc/qir-module/qir-program.hpp"
#include "qir/cc/qir-module/typed-value-prototype.hpp"
#include "qir/cc/qir-module/typed-value.hpp"

#include <iostream>
#include <typeindex>
#include <unordered_map>
#include <vector>
namespace compiler {

struct Qubit
{
  uint64_t id;
};

// Interesting links:
// https://stackoverflow.com/questions/20944533/when-should-i-use-constantdataarray-versus-constantarray
// https://blog.yossarian.net/2020/09/19/LLVMs-getelementptr-by-example
// https://llvm.org/docs/GetElementPtr.html
// https://llvm.org/docs/LangRef.html#getelementptr-instruction
// https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/LangImpl10.html
// https://stackoverflow.com/questions/23330018/llvm-global-integer-array-zeroinitializer
struct LlvmValue
{
  llvm::Value *value;
  //    ConstantArray
};

class QirBuilder
{
public:
  using Type          = llvm::Type;
  using StructType    = llvm::StructType;
  using LlvmContext   = llvm::LLVMContext;
  using LlvmModule    = llvm::Module;
  using Function      = llvm::Function;
  using Constant      = llvm::Constant;
  using ConstantInt   = llvm::ConstantInt;
  using String        = std::string;
  using LlvmFunction  = llvm::Function;
  using LlvmBlock     = llvm::BasicBlock;
  using QirBuilderPtr = std::shared_ptr<QirBuilder>;
  using ValueList     = std::vector<TypedValuePrototypePtr>;

  static QirBuilderPtr create(QirProgram &qir_program, LlvmBlock *block);

  // Quantum operations
  Qubit allocateQubit();
  void  x(Qubit const &q);
  void  z(Qubit const &q);
  void  cnot(Qubit const &q1, Qubit const &q2);

  // Const expressions
  // ConstantBool boolValue(bool const &value);
  ConstantIntegerPtr toInt8(int8_t const &value);
  ConstantIntegerPtr toInt16(int16_t const &value);
  ConstantIntegerPtr toInt32(int32_t const &value);
  ConstantIntegerPtr toInt64(int64_t const &value);

  ConstantArrayPtr constantArray(QirType element_type, ValueList const &values);

  ConstantIntegerPtr constantGetElement(ConstantArrayPtr const   &array,
                                        ConstantIntegerPtr const &index);

  // Memory allocation
  MutableStackVariablePtr newStackVariable(QirType element_type, String const &name);
  MutableStackArrayPtr    newStackArray(QirType element_type, TypedValuePrototypePtr size,
                                        String const &name);

  MutableHeapVariablePtr newHeapVariable(QirType element_type, String const &name);
  MutableHeapArrayPtr    newHeapArray(QirType element_type, TypedValuePrototypePtr size,
                                      String const &name);

  // Arithmetic

  // Terminators
  void returnValue(TypedValuePrototypePtr const &value);

  //

  // Controls

  llvm::IRBuilder<> &builder()
  {
    return builder_;
  }

private:
  explicit QirBuilder(QirProgram &qir_program, LlvmBlock *block);

  bool is_active_{true};

  void requireIsActive();
  void setupBuiltIns();

  QirProgram       &qir_program_;
  llvm::IRBuilder<> builder_;

  LlvmBlock *block_{nullptr};

  // Qubit functions
  LlvmFunction *qubit_allocator_{nullptr};
  LlvmFunction *quantum_x_{nullptr};
  LlvmFunction *quantum_z_{nullptr};
  LlvmFunction *quantum_cnot_{nullptr};

  std::unordered_map<uint64_t, llvm::Value *> qubits_;
  uint64_t                                    qubit_counter_{0};
};

using QirBuilderPtr = QirBuilder::QirBuilderPtr;

}  // namespace compiler