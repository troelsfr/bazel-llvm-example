#pragma once
#include "qir/cc/llvm/llvm-helpers.hpp"
#include "qir/cc/runtime/runtime-type.hpp"
#include "qir/cc/runtime/size-of.hpp"

#include <unordered_map>
namespace compiler {
class Runtime
{
public:
  using String      = std::string;
  using LlvmType    = ConcreteRuntimeType::LlvmType;
  using LlvmContext = llvm::LLVMContext;

  enum
  {
    FIRST_USER_TYPE_ID = 30
  };

  Runtime();

  template <typename T>
  void createNativeType(String const &name, LlvmType *llvm_type = nullptr);

  ConcreteRuntimeType getType(uint64_t const &id);
  ConcreteRuntimeType getType(String const &name);
  ConcreteRuntimeType getType(std::type_index const &type_id);

  std::unique_ptr<LlvmContext> &context()
  {
    return context_;
  }

private:
  std::unordered_map<uint64_t, ConcreteRuntimeType>        types_by_id_{};
  std::unordered_map<String, ConcreteRuntimeType>          types_by_name_{};
  std::unordered_map<std::type_index, ConcreteRuntimeType> types_by_native_id_{};

  uint64_t type_id_counter_{FIRST_USER_TYPE_ID};

  std::unique_ptr<LlvmContext> context_{};
};

template <typename T>
void Runtime::createNativeType(String const &name, LlvmType *llvm_type)
{
  // In case the type is not defined, we define it
  if (llvm_type == nullptr)
  {
    llvm_type = llvm_helpers::LLVMTypeAllocator<T>::type(*context_, name);
  }

  // Creating the type and adding it to the register
  ConcreteRuntimeType new_type{type_id_counter_, llvm_type, std::type_index(typeid(T)),
                               details::SizeOf<T>::value(), name};

  types_by_id_[new_type.id];
  types_by_name_[name] = new_type;
  types_by_native_id_[new_type.native_type_id];

  ++type_id_counter_;
}

}  // namespace compiler