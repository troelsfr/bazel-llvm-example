// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "svadilfari/cc/runtime/runtime.hpp"

#include <functional>
#include <typeindex>

namespace compiler {

RuntimeDefinition::RuntimeDefinition()
{
  // clang-format off
  type_id_to_native_type_ = {
    {TYPE_VOID    , std::type_index(typeid(void))},
    {TYPE_BOOL    , std::type_index(typeid(bool))},
    {TYPE_INT8    , std::type_index(typeid(int8_t))},
    {TYPE_INT16   , std::type_index(typeid(int16_t))},
    {TYPE_INT32   , std::type_index(typeid(int32_t))},
    {TYPE_INT64   , std::type_index(typeid(int64_t))},
    {TYPE_UINT8   , std::type_index(typeid(uint8_t))},
    {TYPE_UINT16  , std::type_index(typeid(uint16_t))},
    {TYPE_UINT32  , std::type_index(typeid(uint32_t))},
    {TYPE_UINT64  , std::type_index(typeid(uint64_t))},
    {TYPE_FLOAT32 , std::type_index(typeid(float))},
    {TYPE_FLOAT64 , std::type_index(typeid(double))}
 };

  size_table_ = {
    {TYPE_VOID    , details::SizeOf<void>::value()},
    {TYPE_BOOL    , details::SizeOf<bool>::value()},
    {TYPE_INT8    , details::SizeOf<int8_t>::value()},
    {TYPE_INT16   , details::SizeOf<int16_t>::value()},
    {TYPE_INT32   , details::SizeOf<int32_t>::value()},
    {TYPE_INT64   , details::SizeOf<int64_t>::value()},
    {TYPE_UINT8   , details::SizeOf<uint8_t>::value()},
    {TYPE_UINT16  , details::SizeOf<uint16_t>::value()},
    {TYPE_UINT32  , details::SizeOf<uint32_t>::value()},
    {TYPE_UINT64  , details::SizeOf<uint64_t>::value()},
    {TYPE_FLOAT32 , details::SizeOf<float>::value()},
    {TYPE_FLOAT64 , details::SizeOf<double>::value()}
 };

  type_id_to_initiator_ =  {
    {TYPE_VOID    , [](llvm::LLVMContext *context, String const &name) {
      return llvm_helpers::LLVMTypeAllocator<void>::type(*context, name);
    }},
    {TYPE_BOOL    , [](llvm::LLVMContext *context, String const &name) {
      return llvm_helpers::LLVMTypeAllocator<bool>::type(*context, name);
    }},
    {TYPE_INT8    , [](llvm::LLVMContext *context, String const &name) {
      return llvm_helpers::LLVMTypeAllocator<int8_t>::type(*context, name);
    }},
    {TYPE_INT16   , [](llvm::LLVMContext *context, String const &name) {
      return llvm_helpers::LLVMTypeAllocator<int16_t>::type(*context, name);
    }},
    {TYPE_INT32   , [](llvm::LLVMContext *context, String const &name) {
      return llvm_helpers::LLVMTypeAllocator<int32_t>::type(*context, name);
    }},
    {TYPE_INT64   , [](llvm::LLVMContext *context, String const &name) {
      return llvm_helpers::LLVMTypeAllocator<int64_t>::type(*context, name);
    }},
    {TYPE_UINT8   , [](llvm::LLVMContext *context, String const &name) {
      return llvm_helpers::LLVMTypeAllocator<uint8_t>::type(*context, name);
    }},
    {TYPE_UINT16  , [](llvm::LLVMContext *context, String const &name) {
      return llvm_helpers::LLVMTypeAllocator<uint16_t>::type(*context, name);
    }},
    {TYPE_UINT32  , [](llvm::LLVMContext *context, String const &name) {
      return llvm_helpers::LLVMTypeAllocator<uint32_t>::type(*context, name);
    }},
    {TYPE_UINT64  , [](llvm::LLVMContext *context, String const &name) {
      return llvm_helpers::LLVMTypeAllocator<uint64_t>::type(*context, name);
    }},
    {TYPE_FLOAT32 , [](llvm::LLVMContext *context, String const &name) {
      return llvm_helpers::LLVMTypeAllocator<float>::type(*context, name);
    }},
    {TYPE_FLOAT64 , [](llvm::LLVMContext *context, String const &name) {
      return llvm_helpers::LLVMTypeAllocator<double>::type(*context, name);
    }},
 };
 // clang-format on  

 for(auto kv: type_id_to_native_type_)
 {
    native_type_to_type_id_[kv.second] = kv.first;
 }
}
void RuntimeDefinition::defineType(PrimitiveTypes const &primitive_type, String const &name)
{
  uint64_t id  = static_cast<uint64_t>(primitive_type);  

  auto     tid = std::type_index(typeid(nullptr_t));

  auto it = type_id_to_native_type_.find(id);
  if(it != type_id_to_native_type_.end())
  {
    tid = it->second;
  }

  uint64_t          size      = 0;
  auto it2 = size_table_.find(id);
  if(it2 == size_table_.end())
  {
    throw std::runtime_error("Unable to find size of type");
  }
  size = it2->second;


  LlvmInitiator initiator{nullptr};
  auto it3 = type_id_to_initiator_.find(id);
  if(it3 == type_id_to_initiator_.end())
  {
    throw std::runtime_error("Unable to find type initiator");
  }
  initiator = it3->second;

  TypeDeclaration type{id, nullptr, tid, size, name, initiator};
  type_register_[name]                      = type;
  native_type_to_name_[type.native_type_id] = name;
}

RuntimeDefinition::TypeDeclarationRegister const &RuntimeDefinition::typeRegister() const
{
  return type_register_;
}

RuntimeDefinition::RuntimeFunctions const &RuntimeDefinition::runtimeFunctions() const
{
  return runtime_functions_;
}

void RuntimeDefinition::declareFunction(String const &name, String const &ret, Strings const &argument_types)
{
  if (runtime_functions_.find(name) != runtime_functions_.end())
  {
    throw std::runtime_error("Function " + name + " already exists.");
  }

  RuntimeFunctionDeclaration decl;
  decl.name                = name;
  decl.return_type         = ret;
  decl.argument_types = argument_types;
  runtime_functions_[name] = decl;
}

}  // namespace compiler