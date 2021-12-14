#pragma once
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "svadilfari/cc/llvm/llvm.hpp"
#include "svadilfari/cc/qir-module/constant-int.hpp"
#include "svadilfari/cc/qir-module/function-declaration.hpp"
#include "svadilfari/cc/qir-module/scope-register.hpp"
#include "svadilfari/cc/qir-module/typed-value.hpp"
#include "svadilfari/cc/runtime/runtime-definition.hpp"
#include "svadilfari/cc/runtime/runtime-type.hpp"
#include "svadilfari/cc/runtime/script-builder-interface.hpp"

#include <iostream>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <vector>
namespace compiler {

class ScopeBuilder;

class ScriptBuilder : public ScriptBuilderInterface
{
public:
  using Type                    = llvm::Type;
  using StructType              = llvm::StructType;
  using LlvmContext             = llvm::LLVMContext;
  using LlvmModule              = llvm::Module;
  using Function                = llvm::Function;
  using Constant                = llvm::Constant;
  using ConstantInt             = llvm::ConstantInt;
  using String                  = std::string;
  using LlvmFunction            = llvm::Function;
  using LlvmBlock               = llvm::BasicBlock;
  using Arguments               = std::vector<TypeDeclaration>;
  using ScopeBuilderPtr         = std::shared_ptr<ScopeBuilder>;
  using Builders                = std::unordered_set<ScopeBuilder *>;
  using ArgTypeNames            = FunctionDeclaration::ArgTypeNames;
  using FunctionCache           = std::unordered_map<String, FunctionDeclaration>;
  using TypeDeclarationRegister = RuntimeDefinition::TypeDeclarationRegister;

  ScriptBuilder(RuntimeDefinition const &runtime_definition);
  ~ScriptBuilder();

  LlvmContext *context();
  LlvmModule  *module();

  TypeDeclaration const &getType(String const &name) const;
  TypeDeclaration const &getType(std::type_index const &type_id) const;

  Type *getLlvmType(String const &name);

  //  ScopeBuilderPtr newFunction(String const &name, TypeDeclaration return_type =
  //  TypeDeclaration(),
  //                            Arguments args = Arguments());
  ScopeBuilderPtr newFunction(String const &name, String const &return_type = "Void",
                              ArgTypeNames const &arguments = {});

  String getQir();

  void addBuilder(ScopeBuilder *builder);
  void removeBuilder(ScopeBuilder *builder);
  void finalise();

  Function *getOrDeclareFunction(String const &name, String const &return_type = "Void",
                                 ArgTypeNames const &arguments = {});

  FunctionDeclaration getFunctionByLlvmName(String const &name);

  ConstantIntegerPtr toInt8(llvm::IRBuilder<> &builder, int8_t const &value);
  ConstantIntegerPtr toInt16(llvm::IRBuilder<> &builder, int16_t const &value);
  ConstantIntegerPtr toInt32(llvm::IRBuilder<> &builder, int32_t const &value);
  ConstantIntegerPtr toInt64(llvm::IRBuilder<> &builder, int64_t const &value);

  ScopeRegister &scope()
  {
    return *scope_;
  }

  std::unique_ptr<LlvmContext> context_{};
  std::unique_ptr<LlvmModule>  module_{};

protected:
  void declareFunction(String const &name, String const &return_type = "Void",
                       ArgTypeNames const &arguments = {}) override;

private:
  void registerType(TypeDeclaration const &type);

  TypeDeclarationRegister                              types_{};
  std::unordered_map<std::type_index, TypeDeclaration> from_native_types_{};

  Builders         builders_{};
  ScopeRegisterPtr scope_{nullptr};

  FunctionCache function_declaration_cache_{};
};

}  // namespace compiler