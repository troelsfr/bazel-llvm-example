#pragma once
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "qir/cc/llvm/llvm.hpp"

#include <type_traits>
#include <typeindex>
namespace compiler {

template <class T, class Val>
struct AssignmentExists
{
  template <class U, class V>
  static auto test(U *) -> decltype(std::declval<U>() = std::declval<V>());
  template <typename, typename>
  static auto test(...) -> std::false_type;

  enum
  {
    value = std::is_same<Val, typename std::decay<decltype(test<T, Val>(0))>::type>::value ||
            std::is_same<T, typename std::decay<decltype(test<T, Val>(0))>::type>::value
  };
};

template <typename T>
class ValueContianer
{
public:
  using Pointer                          = std::shared_ptr<T>;
  ValueContianer()                       = default;
  ValueContianer(ValueContianer const &) = default;
  ValueContianer(ValueContianer &&)      = default;
  ValueContianer(T *ptr)
    : ptr_{ptr}
  {}

  template <typename S>
  operator ValueContianer<S>() const
  {
    ValueContianer<S> ret;
    ret.ptr_ = static_cast<typename ValueContianer<S>::Pointer>(ptr_);
    return ret;
  }

  void reset(T *ptr)
  {
    ptr_.reset(ptr);
  }

  T *operator->() const
  {
    return ptr_.get();
  }

  T *get() const
  {
    return ptr_.get();
  }

  template <typename S>
  typename std::enable_if<AssignmentExists<T, ValueContianer<S>>::value, ValueContianer>::type
  operator=(ValueContianer<S> const &other)
  {
    *ptr_ = other;
    return *this;
  }

  ValueContianer &operator=(ValueContianer const &) = default;
  ValueContianer &operator=(ValueContianer &&) = default;

private:
  Pointer ptr_;
  template <typename S>
  friend class ValueContianer;
};

}  // namespace compiler