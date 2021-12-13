#pragma once
#include "qir/cc/llvm/llvm-helpers.hpp"
#include "qir/cc/llvm/llvm.hpp"
#include "qir/cc/runtime/runtime-type.hpp"
#include "qir/cc/runtime/size-of.hpp"
#include "qir/cc/runtime/type-register.hpp"

#include <functional>
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
  using TypeDeclarationRegister = std::unordered_map<String, TypeDeclaration>;
  using NativeTypeToName        = std::unordered_map<std::type_index, String>;
  //  using Type

  TypeDeclarationRegister const &typeRegister() const;
  RuntimeFunctions const        &runtimeFunctions() const;

  template <typename T>
  void defineType(String const &name);

private:
  TypeDeclarationRegister type_register_{};
  NativeTypeToName        native_type_to_name_{};
  RuntimeFunctions        runtime_functions_{};

  uint64_t type_id_counter_{1};
};

template <typename T>
void RuntimeDefinition::defineType(String const &name)
{
  TypeDeclaration::LlvmInitiator initiator = [](llvm::LLVMContext *context, String const &name) {
    return llvm_helpers::LLVMTypeAllocator<T>::type(*context, name);
  };

  TypeDeclaration type{
      type_id_counter_, nullptr, std::type_index(typeid(T)), details::SizeOf<T>::value(), name,
      initiator};
  type_register_[name] = type;

  ++type_id_counter_;
}

}  // namespace compiler