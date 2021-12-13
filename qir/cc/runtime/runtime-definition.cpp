#include "qir/cc/runtime/runtime.hpp"
namespace compiler {

RuntimeDefinition::TypeDeclarationRegister const &RuntimeDefinition::typeRegister() const
{
  return type_register_;
}

RuntimeDefinition::RuntimeFunctions const &RuntimeDefinition::runtimeFunctions() const
{
  return runtime_functions_;
}

}  // namespace compiler