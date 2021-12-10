#include "qir/cc/runtime/runtime.hpp"
namespace {
// TODO: Remove
struct Qubit
{
};
}  // namespace
namespace compiler {
Runtime::Runtime()
  : context_{std::make_unique<LlvmContext>()}
{
  createNativeType<int8_t>("Int8");
  createNativeType<int16_t>("Int16");
  createNativeType<int32_t>("Int32");
  createNativeType<int64_t>("Int64");
  createNativeType<void>("Unit");
  createNativeType<Qubit>("Qubit");
}

ConcreteRuntimeType Runtime::getType(uint64_t const &id)
{
  auto it = types_by_id_.find(id);
  if (it == types_by_id_.end())
  {
    throw std::runtime_error("Type (by id) not found.");
  }

  return it->second;
}

ConcreteRuntimeType Runtime::getType(String const &name)
{
  auto it = types_by_name_.find(name);
  if (it == types_by_name_.end())
  {
    throw std::runtime_error("Type " + name + " not found.");
  }

  return it->second;
}

ConcreteRuntimeType Runtime::getType(std::type_index const &type_id)
{
  auto it = types_by_native_id_.find(type_id);
  if (it == types_by_native_id_.end())
  {
    throw std::runtime_error("Type not found.");
  }

  return it->second;
}

}  // namespace compiler