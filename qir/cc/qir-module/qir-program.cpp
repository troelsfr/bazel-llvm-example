#include "qir/cc/qir-module/qir-program.hpp"

#include "qir/cc/llvm/llvm.hpp"
#include "qir/cc/qir-module/qir-builder.hpp"

#include <iostream>
#include <unordered_map>
#include <vector>
namespace compiler {

QirProgram::QirProgram()
  : context_{std::make_unique<LlvmContext>()}
  , module_(std::make_unique<LlvmModule>("qir.ll", *context_))
  , scope_{QirScope::create()}
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

void QirProgram::registerType(QirType const &type)
{
  types_[type.name]                = type;
  from_native_types_[type.type_id] = type;
}

QirProgram::~QirProgram()
{
  finalise();
}

QirProgram::LlvmContext *QirProgram::context()
{
  return context_.get();
}

QirProgram::LlvmModule *QirProgram::module()
{
  return module_.get();
}

QirType QirProgram::getType(String const &name)
{
  auto it = types_.find(name);
  if (it == types_.end())
  {
    throw std::runtime_error("Type " + name + " not found.");
  }

  return it->second;
}

QirType QirProgram::getType(std::type_index const &type_id)
{
  auto it = from_native_types_.find(type_id);
  if (it == from_native_types_.end())
  {
    throw std::runtime_error("Type not found.");
  }

  return it->second;
}

QirProgram::Type *QirProgram::getLlvmType(String const &name)
{
  auto it = types_.find(name);
  if (it == types_.end())
  {
    throw std::runtime_error("Type " + name + " not found.");
  }

  return it->second.value;
}

/*
QirBuilderPtr QirProgram::newFunction(String const &name, QirType return_type, Arguments args)
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
  return QirBuilder::create(*this, block);
}
*/

QirBuilderPtr QirProgram::newFunction(String const &name, String const &return_type,
                                      ArgTypeNames const &arguments)
{
  auto function = getOrDeclareFunction(name, return_type, arguments);
  llvm::errs() << "Declaring " << name << ": " << *function << "\n";
  auto block = llvm::BasicBlock::Create(*context_, "entry", function);
  return QirBuilder::create(*this, scope_->childScope(), block);
}

QirProgram::String QirProgram::getQir()
{
  finalise();
  // TODO: Move to program
  String                   str;
  llvm::raw_string_ostream ostream(str);
  ostream << *module_;
  ostream.flush();
  return str;
}

void QirProgram::removeBuilder(QirBuilder *builder)
{
  builders_.erase(builder);
}

void QirProgram::addBuilder(QirBuilder *builder)
{
  builders_.insert(builder);
}

void QirProgram::finalise()
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

FunctionDeclaration QirProgram::getFunctionByLlvmName(String const &name)
{
  auto it = function_declaration_cache_.find(name);
  if (it == function_declaration_cache_.end())
  {
    return {nullptr};
  }
  return it->second;
}

QirProgram::Function *QirProgram::getOrDeclareFunction(String const       &name,
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

ConstantIntegerPtr QirProgram::toInt8(llvm::IRBuilder<> &builder, int8_t const &value)
{
  return ConstantInteger::createNew<int8_t>(builder, static_cast<uint64_t>(value));
}

ConstantIntegerPtr QirProgram::toInt16(llvm::IRBuilder<> &builder, int16_t const &value)
{
  return ConstantInteger::createNew<int16_t>(builder, static_cast<uint64_t>(value));
}

ConstantIntegerPtr QirProgram::toInt32(llvm::IRBuilder<> &builder, int32_t const &value)
{
  return ConstantInteger::createNew<int32_t>(builder, static_cast<uint64_t>(value));
}

ConstantIntegerPtr QirProgram::toInt64(llvm::IRBuilder<> &builder, int64_t const &value)
{
  return ConstantInteger::createNew<int64_t>(builder, static_cast<uint64_t>(value));
}

}  // namespace compiler