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

#include <functional>
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
// https://stackoverflow.com/questions/42255453/how-do-i-initialize-an-integer-array-in-llvm-using-a-list-of-integers
struct LlvmValue
{
  llvm::Value *value;
  //    ConstantArray
};

class IfStatement;
using IfStatementPtr = std::shared_ptr<IfStatement>;

class QirBuilder
{
public:
  using Type               = llvm::Type;
  using StructType         = llvm::StructType;
  using LlvmContext        = llvm::LLVMContext;
  using LlvmModule         = llvm::Module;
  using Function           = llvm::Function;
  using Constant           = llvm::Constant;
  using ConstantInt        = llvm::ConstantInt;
  using String             = std::string;
  using LlvmFunction       = llvm::Function;
  using LlvmBlock          = llvm::BasicBlock;
  using QirBuilderPtr      = std::shared_ptr<QirBuilder>;
  using ValueList          = std::vector<TypedValuePrototypePtr>;
  using Builder            = llvm::IRBuilder<>;
  using TerminatorFunction = std::function<void(Builder &)>;

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

  TypedValuePtr constantGetElement(ConstantArrayPtr const &array, ConstantIntegerPtr const &index);

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
  IfStatementPtr ifStatement(TypedValuePrototypePtr const &value);

  llvm::IRBuilder<> &builder()
  {
    return builder_;
  }

  void finalise()
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

  void setTerrminatorFunction(TerminatorFunction const &f)
  {
    add_terminator_ = f;
  }

  ~QirBuilder();

protected:
  explicit QirBuilder(QirProgram &qir_program, LlvmBlock *block);
  QirProgram &qir_program_;
  Builder     builder_;
  LlvmBlock  *block_{nullptr};

  TerminatorFunction add_terminator_{nullptr};
  bool               isActive();

private:
  bool finalise_called_{false};

  void requireIsActive();
  void setupBuiltIns();

  // Qubit functions
  LlvmFunction *qubit_allocator_{nullptr};
  LlvmFunction *quantum_x_{nullptr};
  LlvmFunction *quantum_z_{nullptr};
  LlvmFunction *quantum_cnot_{nullptr};

  std::unordered_map<uint64_t, llvm::Value *> qubits_;
  uint64_t                                    qubit_counter_{0};
};

using QirBuilderPtr = QirBuilder::QirBuilderPtr;

class ElseStatement : public QirBuilder
{
public:
  using ElseStatementPtr = std::shared_ptr<ElseStatement>;

  static ElseStatementPtr create(QirProgram &qir_program, LlvmBlock *block, LlvmBlock *final_block)
  {
    ElseStatementPtr ret;
    ret.reset(new ElseStatement(qir_program, block, final_block));
    return ret;
  }

protected:
  ElseStatement(QirProgram &qir_program, LlvmBlock *block, LlvmBlock *final_block)
    : QirBuilder(qir_program, block)
    , final_block_{final_block}
  {
    setTerrminatorFunction([final_block](Builder &builder) { builder.CreateBr(final_block); });
  }

  LlvmBlock *final_block_{nullptr};
};
using ElseStatementPtr = ElseStatement::ElseStatementPtr;

class IfStatement : public ElseStatement
{
public:
  using IfStatementPtr = std::shared_ptr<IfStatement>;

  static IfStatementPtr create(QirProgram &qir_program, LlvmBlock *block, LlvmBlock *final_block)
  {
    IfStatementPtr ret;
    ret.reset(new IfStatement(qir_program, block, final_block));
    return ret;
  }

  ElseStatementPtr elseStatement()
  {
    auto else_block = llvm::BasicBlock::Create(*qir_program_.context(), "else",
                                               final_block_->getParent(), final_block_);

    final_block_->replaceUsesWithIf(else_block, [](llvm::Use &use) {
      auto *phi_node = llvm::dyn_cast<llvm::PHINode>(use.getUser());
      return (phi_node == nullptr);
    });

    builder_.CreateBr(final_block_);
    assert(!isActive());

    return ElseStatement::create(qir_program_, else_block, final_block_);
  }

protected:
  using ElseStatement::ElseStatement;
};

}  // namespace compiler