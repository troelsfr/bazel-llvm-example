#pragma once
#include "qir/cc/llvm/llvm.hpp"
#include "qir/cc/qir-module/typed-value-prototype.hpp"

#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace compiler {

class QirScope : public std::enable_shared_from_this<QirScope>
{
public:
  using String                   = std::string;
  using QirScopePtr              = std::shared_ptr<QirScope>;
  using VariablesDeclaredInScope = std::unordered_set<String>;
  static QirScopePtr create(QirScopePtr parent = nullptr)
  {
    QirScopePtr ret;
    ret.reset(new QirScope(parent));
    return ret;
  }

  QirScopePtr childScope()
  {
    return QirScope::create(shared_from_this());
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
  QirScope(QirScopePtr parent)
    : parent_{parent}
  {
    if (parent_ != nullptr)
    {
      used_names_ = parent_->used_names_;
    }
  }

  std::unordered_set<String>                         used_names_;
  std::unordered_map<String, TypedValuePrototypePtr> named_values_;

  QirScopePtr parent_{nullptr};
};
using QirScopePtr = QirScope::QirScopePtr;

}  // namespace compiler