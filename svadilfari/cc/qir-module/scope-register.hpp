#pragma once
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "svadilfari/cc/llvm/llvm.hpp"
#include "svadilfari/cc/qir-module/typed-value-prototype.hpp"

#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace compiler {

class ScopeRegister : public std::enable_shared_from_this<ScopeRegister>
{
public:
  using String                   = std::string;
  using ScopeRegisterPtr         = std::shared_ptr<ScopeRegister>;
  using VariablesDeclaredInScope = std::unordered_set<String>;
  static ScopeRegisterPtr create(ScopeRegisterPtr parent = nullptr)
  {
    ScopeRegisterPtr ret;
    ret.reset(new ScopeRegister(parent));
    return ret;
  }

  ScopeRegisterPtr childScope()
  {
    return ScopeRegister::create(shared_from_this());
  }

  bool has(String const &name)
  {
    return used_names_.find(name) != used_names_.end();
  }

  void insert(String const &name, TypedValuePrototypePtr const &value)
  {
    used_names_.insert(name);
    named_values_[name] = value;
  }

  TypedValuePrototypePtr get(String const &name)
  {
    auto it = named_values_.find(name);
    if (it == named_values_.end())
    {
      if (parent_ == nullptr)
      {
        throw std::runtime_error("Named variable not found");
      }

      return parent_->get(name);
    }

    return it->second;
  }

private:
  ScopeRegister(ScopeRegisterPtr parent)
    : parent_{parent}
  {
    if (parent_ != nullptr)
    {
      used_names_ = parent_->used_names_;
    }
  }

  std::unordered_set<String>                         used_names_;
  std::unordered_map<String, TypedValuePrototypePtr> named_values_;

  ScopeRegisterPtr parent_{nullptr};
};
using ScopeRegisterPtr = ScopeRegister::ScopeRegisterPtr;

}  // namespace compiler