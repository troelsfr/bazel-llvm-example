#pragma once

#include "qir/cc/llvm/llvm.hpp"

#include <cstring>
#include <vector>

namespace compiler {

class ScriptBuilderInterface
{
public:
  using String                      = std::string;
  using ArgTypeNames                = std::vector<String>;
  using Function                    = llvm::Function;
  ScriptBuilderInterface()          = default;
  virtual ~ScriptBuilderInterface() = default;

  virtual void declareFunction(String const &name, String const &return_type = "Void",
                               ArgTypeNames const &arguments = {}) = 0;
};

}  // namespace compiler