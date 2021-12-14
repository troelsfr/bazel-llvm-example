#pragma once
#include "qir/cc/llvm/llvm-helpers.hpp"
#include "qir/cc/llvm/llvm.hpp"
#include "qir/cc/runtime/runtime-type.hpp"
#include "qir/cc/runtime/size-of.hpp"
#include "qir/cc/runtime/type-register.hpp"

#include <functional>
#include <iostream>
#include <typeindex>
#include <unordered_map>
#include <vector>
namespace compiler {
struct FunctionDefinition
{
  using String  = std::string;
  using Strings = std::vector<String>;

  String  name{""};
  String  return_type{""};
  Strings argument_types{};
};

class RuntimeDefinition
{
public:
  using String                  = std::string;
  using RuntimeFunctions        = std::unordered_map<String, FunctionDefinition>;
  using LlvmContext             = llvm::LLVMContext;
  using LlvmModule              = llvm::Module;
  using LlvmInitiator           = TypeDeclaration::LlvmInitiator;
  using TypeDeclarationRegister = std::unordered_map<String, TypeDeclaration>;
  using NativeTypeToName        = std::unordered_map<std::type_index, String>;
  using TypeIdToNativeType      = std::unordered_map<uint64_t, std::type_index>;
  using NativeTypeToTypeId      = std::unordered_map<std::type_index, uint64_t>;
  using TypeIdToLlvmInitiator   = std::unordered_map<uint64_t, LlvmInitiator>;
  using SizeTable               = std::unordered_map<uint64_t, uint64_t>;

  enum PrimitiveTypes
  {
    TYPE_VOID    = 1,
    TYPE_BOOL    = 2,
    TYPE_INT8    = 5,
    TYPE_INT16   = 6,
    TYPE_INT32   = 7,
    TYPE_INT64   = 8,
    TYPE_UINT8   = 9,
    TYPE_UINT16  = 10,
    TYPE_UINT32  = 11,
    TYPE_UINT64  = 12,
    TYPE_FLOAT32 = 15,
    TYPE_FLOAT64 = 16,

    USER_TYPE_OFFSET = 30
  };

  RuntimeDefinition();

  TypeDeclarationRegister const &typeRegister() const;
  RuntimeFunctions const        &runtimeFunctions() const;

  template <typename T>
  void defineType(String const &name);
  void defineType(PrimitiveTypes const &primitive_type, String const &name);

  template <typename R, typename... Args>
  void declareFunction(String const &name);
  template <typename R>
  void declareFunction(String const &name);

  String getTypeName(std::type_index const &ti)
  {
    auto it = native_type_to_name_.find(ti);
    if (it == native_type_to_name_.end())
    {
      throw std::runtime_error("Type " + static_cast<std::string>(ti.name()) + " not found.");
    }

    return it->second;
  }

private:
  TypeDeclarationRegister type_register_{};
  NativeTypeToName        native_type_to_name_{};
  RuntimeFunctions        runtime_functions_{};

  TypeIdToNativeType    type_id_to_native_type_{};
  TypeIdToLlvmInitiator type_id_to_initiator_{};
  NativeTypeToTypeId    native_type_to_type_id_{};
  SizeTable             size_table_{};

  uint64_t type_id_counter_{static_cast<uint64_t>(USER_TYPE_OFFSET)};
};

template <typename T>
void RuntimeDefinition::defineType(String const &name)
{
  uint64_t id  = type_id_counter_;
  auto     tid = std::type_index(typeid(T));

  auto it = native_type_to_type_id_.find(tid);
  if (it == native_type_to_type_id_.end())
  {
    ++type_id_counter_;
  }
  else
  {
    id = it->second;
  }

  uint64_t      size      = details::SizeOf<T>::value();
  LlvmInitiator initiator = [](llvm::LLVMContext *context, String const &name) {
    return llvm_helpers::LLVMTypeAllocator<T>::type(*context, name);
  };

  TypeDeclaration type{id, nullptr, tid, size, name, initiator};
  type_register_[name]                      = type;
  native_type_to_name_[type.native_type_id] = name;
}

template <typename N, typename... Args>
struct ResolveArgumentTypes
{
  static void apply(RuntimeDefinition &rd, FunctionDefinition::Strings &argument_types)
  {
    auto name = rd.getTypeName(std::type_index(typeid(N)));
    argument_types.push_back(name);
    ResolveArgumentTypes<Args...>::apply(rd, argument_types);
  }
};

template <typename N>
struct ResolveArgumentTypes<N>
{
  static void apply(RuntimeDefinition &rd, FunctionDefinition::Strings &argument_types)
  {
    auto name = rd.getTypeName(std::type_index(typeid(N)));
    argument_types.push_back(name);
  }
};

template <typename R, typename... Args>
void RuntimeDefinition::declareFunction(String const &name)
{
  if (runtime_functions_.find(name) != runtime_functions_.end())
  {
    throw std::runtime_error("Function " + name + " already exists.");
  }

  FunctionDefinition decl;

  decl.name        = name;
  decl.return_type = getTypeName(std::type_index(typeid(R)));

  ResolveArgumentTypes<Args...>::apply(*this, decl.argument_types);
  runtime_functions_[name] = decl;
}

template <typename R>
void RuntimeDefinition::declareFunction(String const &name)
{
  if (runtime_functions_.find(name) != runtime_functions_.end())
  {
    throw std::runtime_error("Function " + name + " already exists.");
  }

  FunctionDefinition decl;
  decl.name                = name;
  decl.return_type         = getTypeName(std::type_index(typeid(R)));
  runtime_functions_[name] = decl;
}

}  // namespace compiler