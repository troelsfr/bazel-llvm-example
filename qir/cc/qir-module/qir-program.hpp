#pragma once
#include "qir/cc/llvm/llvm.hpp"
#include "qir/cc/qir-module/typed-value.hpp"

#include <iostream>
#include <typeindex>
#include <unordered_map>
#include <vector>
namespace compiler {

class QirBuilder;

struct QirType
{
  using Type = llvm::Type;
  Type           *value{nullptr};
  std::type_index type_id{std::type_index(typeid(void))};
  int64_t         size{sizeof(int64_t)};
};

class QirProgram
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
  using Arguments     = std::vector<QirType>;
  using QirBuilderPtr = std::shared_ptr<QirBuilder>;

  QirProgram();

  LlvmContext *context();
  LlvmModule  *module();

  QirType getType(String const &name);
  Type   *getLlvmType(String const &name);

  QirBuilderPtr newFunction(String const &name, QirType return_type = QirType(),
                            Arguments args = Arguments());

  String getQir();

private:
  std::unique_ptr<LlvmContext> context_{};
  std::unique_ptr<LlvmModule>  module_{};

  std::unordered_map<String, QirType> types_{};
};

}  // namespace compiler