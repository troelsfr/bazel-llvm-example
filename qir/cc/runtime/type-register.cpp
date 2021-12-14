// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "qir/cc/runtime/runtime.hpp"

namespace {
// TODO: Remove
struct Qubit
{
};
}  // namespace
namespace compiler {
TypeDeclarationRegister ::TypeDeclarationRegister()
{
  declareNativeType<int8_t>("Int8");
  declareNativeType<int16_t>("Int16");
  declareNativeType<int32_t>("Int32");
  declareNativeType<int64_t>("Int64");
  declareNativeType<void>("Unit");
  declareNativeType<Qubit>("Qubit");
}

TypeDeclaration TypeDeclarationRegister::getType(uint64_t const &id)
{
  auto it = types_by_id_.find(id);
  if (it == types_by_id_.end())
  {
    throw std::runtime_error("Type (by id) not found.");
  }

  return it->second;
}

TypeDeclaration TypeDeclarationRegister::getType(String const &name)
{
  auto it = types_by_name_.find(name);
  if (it == types_by_name_.end())
  {
    throw std::runtime_error("Type " + name + " not found.");
  }

  return it->second;
}

TypeDeclaration TypeDeclarationRegister::getType(std::type_index const &type_id)
{
  auto it = types_by_native_id_.find(type_id);
  if (it == types_by_native_id_.end())
  {
    throw std::runtime_error("Type not found.");
  }

  return it->second;
}

}  // namespace compiler