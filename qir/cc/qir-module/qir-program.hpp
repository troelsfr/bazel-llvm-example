#pragma once
#include "qir/cc/llvm/llvm.hpp"
#include "qir/cc/qir-module/typed-value.hpp"

#include <iostream>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
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

struct FunctionDeclaration
{
  using String       = std::string;
  using Function     = llvm::Function;
  using ArgTypeNames = std::vector<String>;

  Function    *function{nullptr};
  String       name{};
  String       return_type{};
  ArgTypeNames argument_types{};
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
  using Builders      = std::unordered_set<QirBuilder *>;
  using ArgTypeNames  = FunctionDeclaration::ArgTypeNames;
  using FunctionCache = std::unordered_map<String, FunctionDeclaration>;

  QirProgram();
  ~QirProgram();

  LlvmContext *context();
  LlvmModule  *module();

  QirType getType(String const &name);
  Type   *getLlvmType(String const &name);

  QirBuilderPtr newFunction(String const &name, QirType return_type = QirType(),
                            Arguments args = Arguments());

  String getQir();

  void addBuilder(QirBuilder *builder);
  void removeBuilder(QirBuilder *builder);
  void finalise();

  Function *getOrDeclareFunction(String name, String const &return_type = "Void",
                                 ArgTypeNames const &arguments = {});

private:
  std::unique_ptr<LlvmContext> context_{};
  std::unique_ptr<LlvmModule>  module_{};

  std::unordered_map<String, QirType> types_{};
  Builders                            builders_{};

  FunctionCache function_declaration_cache_{};
};

}  // namespace compiler