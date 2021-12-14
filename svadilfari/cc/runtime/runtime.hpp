#pragma once
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "svadilfari/cc/llvm/llvm-helpers.hpp"
#include "svadilfari/cc/runtime/runtime-definition.hpp"
#include "svadilfari/cc/runtime/runtime-type.hpp"
#include "svadilfari/cc/runtime/size-of.hpp"

#include <iostream>
#include <type_traits>
#include <unordered_map>

namespace compiler {

template <class T>
struct DecomposeLambda;

template <class Ret, class T, class... Args>
struct DecomposeLambda<Ret (T::*)(Args...) const>
{
  using ReturnType       = Ret;
  using ArgumentTypes    = std::tuple<Args...>;
  using Lambda           = std::function<Ret(Args...)>;
  using FunctionPointer  = Ret (*)(Args...);
  constexpr static int n = sizeof...(Args);
};

class Runtime : public RuntimeDefinition
{
public:
  using String           = std::string;
  using Strings          = std::vector<String>;
  using LlvmType         = TypeDeclaration::LlvmType;
  using JITTargetAddress = llvm::JITTargetAddress;

  Runtime();

  void initPlatform();

  template <typename F>
  void defineFunction(String const &name, F &&lambda)
  {
    using FunctionPointer = typename DecomposeLambda<decltype(&F::operator())>::FunctionPointer;
    defineFunctionImpl(name, FunctionPointer(lambda));
  }

  void defineFunction(String const &name, String const &ret, Strings const &arg_types, void *ptr)
  {
    declareFunction(name, ret, arg_types);
    function_addresses_[name] = llvm::pointerToJITTargetAddress(ptr);
  }

  std::unordered_map<String, JITTargetAddress> const &functionAddresses() const
  {
    return function_addresses_;
  }

private:
  template <class R, class... Args>
  void defineFunctionImpl(String const &name, R (*ptr)(Args...))
  {
    declareFunction<R, Args...>(name);
    function_addresses_[name] = llvm::pointerToJITTargetAddress(ptr);
  }

  std::unordered_map<String, JITTargetAddress> function_addresses_;
};

}  // namespace compiler