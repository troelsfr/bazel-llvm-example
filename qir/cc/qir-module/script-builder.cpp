#include "qir/cc/llvm/llvm.hpp"
#include "qir/cc/qir-module/scope-builder.hpp"
#include "qir/cc/qir-module/script-builder.hpp"

#include <iostream>
#include <unordered_map>
#include <vector>
namespace compiler {

ScriptBuilder::ScriptBuilder()
  : context_{std::make_unique<LlvmContext>()}
  , module_(std::make_unique<LlvmModule>("qir.ll", *context_))
  , scope_{ScopeRegister::create()}
{
  registerType({llvm::IntegerType::get(*context_, 8), std::type_index(typeid(int8_t)),
                sizeof(int8_t), "Int8"});
  registerType({llvm::IntegerType::get(*context_, 16), std::type_index(typeid(int16_t)),
                sizeof(int16_t), "Int16"});
  registerType({llvm::IntegerType::get(*context_, 32), std::type_index(typeid(int32_t)),
                sizeof(int32_t), "Int32"});
  registerType({llvm::IntegerType::get(*context_, 64), std::type_index(typeid(int64_t)),
                sizeof(int64_t), "Int64"});
  registerType(
      {llvm::Type::getVoidTy(*context_), std::type_index(typeid(void)), sizeof(int64_t), "Void"});

  registerType({llvm::PointerType::get(llvm::StructType::create(*context_, "Qubit"), 0),
                std::type_index(typeid(Qubit)), sizeof(Qubit), "Qubit"});
}

void ScriptBuilder::registerType(QirType const &type)
{
  types_[type.name]                = type;
  from_native_types_[type.type_id] = type;
}

ScriptBuilder::~ScriptBuilder()
{
  finalise();
}

ScriptBuilder::LlvmContext *ScriptBuilder::context()
{
  return context_.get();
}

ScriptBuilder::LlvmModule *ScriptBuilder::module()
{
  return module_.get();
}

QirType ScriptBuilder::getType(String const &name)
{
  auto it = types_.find(name);
  if (it == types_.end())
  {
    throw std::runtime_error("Type " + name + " not found.");
  }

  return it->second;
}

QirType ScriptBuilder::getType(std::type_index const &type_id)
{
  auto it = from_native_types_.find(type_id);
  if (it == from_native_types_.end())
  {
    throw std::runtime_error("Type not found.");
  }

  return it->second;
}

ScriptBuilder::Type *ScriptBuilder::getLlvmType(String const &name)
{
  auto it = types_.find(name);
  if (it == types_.end())
  {
    throw std::runtime_error("Type " + name + " not found.");
  }

  return it->second.value;
}

/*
ScopeBuilderPtr ScriptBuilder::newFunction(String const &name, QirType return_type, Arguments args)
{
  if (return_type.value == nullptr)
  {
    return_type.value = llvm::Type::getVoidTy(*context_);
  }

  std::vector<Type *> llvm_args{};
  for (auto &t : args)
  {
    llvm_args.push_back(t.value);
  }

  llvm::FunctionType *function_signature =
      llvm::FunctionType::get(return_type.value, llvm_args, false);
  auto function = llvm::Function::Create(
      function_signature, llvm::Function::LinkageTypes::ExternalLinkage, name, module_.get());

  auto block = llvm::BasicBlock::Create(*context_, "entry", function);
  return ScopeBuilder::create(*this, block);
}
*/

ScopeBuilderPtr ScriptBuilder::newFunction(String const &name, String const &return_type,
                                           ArgTypeNames const &arguments)
{
  auto function = getOrDeclareFunction(name, return_type, arguments);
  llvm::errs() << "Declaring " << name << ": " << *function << "\n";
  auto block = llvm::BasicBlock::Create(*context_, "entry", function);
  return ScopeBuilder::create(*this, scope_->childScope(), block);
}

ScriptBuilder::String ScriptBuilder::getQir()
{
  finalise();
  // TODO: Move to program
  String                   str;
  llvm::raw_string_ostream ostream(str);
  ostream << *module_;
  ostream.flush();
  return str;
}

void ScriptBuilder::removeBuilder(ScopeBuilder *builder)
{
  builders_.erase(builder);
}

void ScriptBuilder::addBuilder(ScopeBuilder *builder)
{
  builders_.insert(builder);
}

void ScriptBuilder::finalise()
{
  // We make a copy as calling finalise on the builders
  // will remove them from the list and hence mess the iterators up.
  auto copy = builders_;
  for (auto &b : copy)
  {
    b->finalise();
  }

  assert(builders_.size() == 0);
}

FunctionDeclaration ScriptBuilder::getFunctionByLlvmName(String const &name)
{
  auto it = function_declaration_cache_.find(name);
  if (it == function_declaration_cache_.end())
  {
    return {nullptr};
  }
  return it->second;
}

ScriptBuilder::Function *ScriptBuilder::getOrDeclareFunction(String const       &name,
                                                             String const       &return_type,
                                                             ArgTypeNames const &arguments)
{
  llvm::errs() << "DECLARING " << name << "\n";
  auto it = function_declaration_cache_.find(name);
  if (it != function_declaration_cache_.end())
  {
    // TODO: Validate signature
    return it->second.function;
  }

  FunctionDeclaration decl;
  decl.name           = name;
  decl.return_type    = return_type;
  decl.argument_types = arguments;

  auto                ret_type = getLlvmType(return_type);
  std::vector<Type *> args{};
  for (auto &a : arguments)
  {
    args.push_back(getLlvmType(a));
  }

  llvm::FunctionType *signture = llvm::FunctionType::get(ret_type, args, false);
  decl.function = llvm::Function::Create(signture, llvm::Function::LinkageTypes::ExternalLinkage,
                                         name, *module_);

  function_declaration_cache_[name] = decl;
  return decl.function;
}

ConstantIntegerPtr ScriptBuilder::toInt8(llvm::IRBuilder<> &builder, int8_t const &value)
{
  return ConstantInteger::createNew<int8_t>(builder, static_cast<uint64_t>(value));
}

ConstantIntegerPtr ScriptBuilder::toInt16(llvm::IRBuilder<> &builder, int16_t const &value)
{
  return ConstantInteger::createNew<int16_t>(builder, static_cast<uint64_t>(value));
}

ConstantIntegerPtr ScriptBuilder::toInt32(llvm::IRBuilder<> &builder, int32_t const &value)
{
  return ConstantInteger::createNew<int32_t>(builder, static_cast<uint64_t>(value));
}

ConstantIntegerPtr ScriptBuilder::toInt64(llvm::IRBuilder<> &builder, int64_t const &value)
{
  return ConstantInteger::createNew<int64_t>(builder, static_cast<uint64_t>(value));
}

}  // namespace compiler