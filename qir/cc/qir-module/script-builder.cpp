#include "qir/cc/qir-module/script-builder.hpp"

#include "qir/cc/llvm/llvm.hpp"
#include "qir/cc/qir-module/scope-builder.hpp"

#include <iostream>
#include <unordered_map>
#include <vector>
namespace compiler {
/*
struct TypeDeclaration  // TODO: Replace with concrete runtime type
{
  using String = std::string;
  using Type   = llvm::Type;

  Type           *value{nullptr};
  std::type_index type_id{std::type_index(typeid(nullptr_t))};
  int64_t         size{sizeof(int64_t)};
  String          name;
};
*/

ScriptBuilder::ScriptBuilder(RuntimeDefinition const &runtime_definition)
  : context_{std::make_unique<LlvmContext>()}
  , module_(std::make_unique<LlvmModule>("qir.ll", *context_))
  , scope_{ScopeRegister::create()}
{
  types_ = runtime_definition.typeRegister();
  for (auto &name_type_pair : types_)
  {
    auto &type = name_type_pair.second;
    if (type.initiator)
    {
      type.llvm_type                          = type.initiator(context_.get(), type.name);
      from_native_types_[type.native_type_id] = type;
    }
  }
}

void ScriptBuilder::registerType(TypeDeclaration const &type)
{
  types_[type.name]                       = type;
  from_native_types_[type.native_type_id] = type;
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

TypeDeclaration const &ScriptBuilder::getType(String const &name) const
{
  auto it = types_.find(name);
  if (it == types_.end())
  {
    throw std::runtime_error("Type " + name + " not found.");
  }

  return it->second;
}

TypeDeclaration const &ScriptBuilder::getType(std::type_index const &type_id) const
{
  auto it = from_native_types_.find(type_id);
  if (it == from_native_types_.end())
  {
    throw std::runtime_error("Type (getType via type id) not found.");
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

  return it->second.llvm_type;
}

/*
ScopeBuilderPtr ScriptBuilder::newFunction(String const &name, TypeDeclaration  return_type,
Arguments args)
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

void ScriptBuilder::declareFunction(String const &name, String const &return_type,
                                    ArgTypeNames const &arguments)
{
  auto it = function_declaration_cache_.find(name);
  if (it != function_declaration_cache_.end())
  {
    throw std::runtime_error("Function already declared");
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
}

ScriptBuilder::Function *ScriptBuilder::getOrDeclareFunction(String const       &name,
                                                             String const       &return_type,
                                                             ArgTypeNames const &arguments)
{

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
  return ConstantInteger::createNew<int8_t>(getType(std::type_index(typeid(int8_t))), builder,
                                            static_cast<uint64_t>(value));
}

ConstantIntegerPtr ScriptBuilder::toInt16(llvm::IRBuilder<> &builder, int16_t const &value)
{
  return ConstantInteger::createNew<int16_t>(getType(std::type_index(typeid(int16_t))), builder,
                                             static_cast<uint64_t>(value));
}

ConstantIntegerPtr ScriptBuilder::toInt32(llvm::IRBuilder<> &builder, int32_t const &value)
{
  return ConstantInteger::createNew<int32_t>(getType(std::type_index(typeid(int32_t))), builder,
                                             static_cast<uint64_t>(value));
}

ConstantIntegerPtr ScriptBuilder::toInt64(llvm::IRBuilder<> &builder, int64_t const &value)
{
  return ConstantInteger::createNew<int64_t>(getType(std::type_index(typeid(int64_t))), builder,
                                             static_cast<uint64_t>(value));
}

}  // namespace compiler