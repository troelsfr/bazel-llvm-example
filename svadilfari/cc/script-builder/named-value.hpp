// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

namespace compiler {

class NamedValue : public TypedValue, public std::enable_shared_from_this<NamedValue>
{
public:
  static Wrapper<NamedValue> createNew(std::string const &name)
  {
    Wrapper<NamedValue> ret;
    ret.reset(new NamedValue(name));
    return ret;
  }

  Value *toValue(LLVMContext * /*context*/, Builder & /*builder*/) const override
  {
    // TODO:
    return nullptr;
  }

  std::string toString() const override
  {
    return name_;
  }

  bool isLhs() const override
  {
    return true;
  }

  ConstantIntPtr operator=(ConstantIntPtr const &other)
  {
    /*
    if (typeId() != other.typeId())
    {
      throw std::runtime_error("Cannot implicitly convert types during assignment");
    }
    */
    value_ = other;
    return other;
  }

protected:
  NamedValue(std::string const &name)
    : TypedValue(std::type_index(typeid(void)))
    , name_{name}
  {}
  std::string   name_{};
  TypedValuePtr value_;
};

using NamedValuePtr = Wrapper<NamedValue>;

}  // namespace compiler