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
{
  types_["Int8"]  = {llvm::IntegerType::get(*context_, 8), std::type_index(typeid(int8_t)),
                    sizeof(int8_t)};
  types_["Int16"] = {llvm::IntegerType::get(*context_, 16), std::type_index(typeid(int16_t)),
                     sizeof(int16_t)};
  types_["Int32"] = {llvm::IntegerType::get(*context_, 32), std::type_index(typeid(int32_t)),
                     sizeof(int32_t)};
  types_["Int64"] = {llvm::IntegerType::get(*context_, 64), std::type_index(typeid(int64_t)),
                     sizeof(int64_t)};
  types_["Void"]  = {llvm::Type::getVoidTy(*context_), std::type_index(typeid(void)),
                    sizeof(int64_t)};

  // TODO: Cpp type
  types_["Qubit"] = {llvm::PointerType::get(llvm::StructType::create(*context_, "Qubit"), 0),
                     std::type_index(typeid(void)), sizeof(int64_t)};
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

QirProgram::Type *QirProgram::getLlvmType(String const &name)
{
  auto it = types_.find(name);
  if (it == types_.end())
  {
    throw std::runtime_error("Type " + name + " not found.");
  }

  return it->second.value;
}

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

QirProgram::Function *QirProgram::getOrDeclareFunction(String name, String const &return_type,
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
                                         "__quantum__qis__x__body", *module_);

  function_declaration_cache_[name] = decl;
  return decl.function;
}

}  // namespace compiler