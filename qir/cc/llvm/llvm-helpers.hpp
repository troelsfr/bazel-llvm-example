#pragma once
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "qir/cc/llvm/llvm.hpp"

namespace llvm_helpers {
template <typename T>
struct ValueConverter
{
};

template <>
struct ValueConverter<llvm::Value *>
{
  static llvm::Value *convert(llvm::LLVMContext * /*context*/, llvm::Value *v)
  {
    return v;
  }
};

template <>
struct ValueConverter<bool>
{
  static llvm::Value *convert(llvm::LLVMContext *context, bool v)
  {
    return llvm::ConstantInt::get(*context, llvm::APInt(1, static_cast<uint64_t>(v)));
  }
};

template <>
struct ValueConverter<int8_t>
{
  static llvm::Value *convert(llvm::LLVMContext *context, int8_t v)
  {
    return llvm::ConstantInt::get(*context, llvm::APInt(8, static_cast<uint64_t>(v)));
  }
};

template <>
struct ValueConverter<int16_t>
{
  static llvm::Value *convert(llvm::LLVMContext *context, int16_t v)
  {
    return llvm::ConstantInt::get(*context, llvm::APInt(16, static_cast<uint64_t>(v)));
  }
};

template <>
struct ValueConverter<int32_t>
{
  static llvm::Value *convert(llvm::LLVMContext *context, int32_t v)
  {
    return llvm::ConstantInt::get(*context, llvm::APInt(32, static_cast<uint64_t>(v)));
  }
};

template <>
struct ValueConverter<int64_t>
{
  static llvm::Value *convert(llvm::LLVMContext *context, int64_t v)
  {
    return llvm::ConstantInt::get(*context, llvm::APInt(64, static_cast<uint64_t>(v)));
  }
};

template <typename T>
llvm::Value *convertToValue(llvm::LLVMContext *context, T x)
{
  return ValueConverter<typename std::decay<T>::type>::convert(context, x);
}

template <typename T>
struct LLVMTypeAllocator
{
  static llvm::Type *type(llvm::LLVMContext &context, std::string const &name)
  {
    return llvm::StructType::create(context, name.c_str());
  }
};

template <>
struct LLVMTypeAllocator<void>
{
  static llvm::Type *type(llvm::LLVMContext &context, std::string const &)
  {
    return llvm::Type::getVoidTy(context);
  }
};

template <>
struct LLVMTypeAllocator<bool>
{
  static llvm::Type *type(llvm::LLVMContext &context, std::string const &)
  {
    return llvm::Type::getInt1Ty(context);
  }
};

template <>
struct LLVMTypeAllocator<int8_t>
{
  static llvm::Type *type(llvm::LLVMContext &context, std::string const &)
  {
    return llvm::Type::getInt8Ty(context);
  }
};

template <>
struct LLVMTypeAllocator<int16_t>
{
  static llvm::Type *type(llvm::LLVMContext &context, std::string const &)
  {
    return llvm::Type::getInt16Ty(context);
  }
};

template <>
struct LLVMTypeAllocator<int32_t>
{
  static llvm::Type *type(llvm::LLVMContext &context, std::string const &)
  {
    return llvm::Type::getInt32Ty(context);
  }
};

template <>
struct LLVMTypeAllocator<int64_t>
{
  static llvm::Type *type(llvm::LLVMContext &context, std::string const &)
  {
    return llvm::Type::getInt64Ty(context);
  }
};

template <>
struct LLVMTypeAllocator<float>
{
  static llvm::Type *type(llvm::LLVMContext &context, std::string const &)
  {
    return llvm::Type::getFloatTy(context);
  }
};

template <>
struct LLVMTypeAllocator<double>
{
  static llvm::Type *type(llvm::LLVMContext &context, std::string const &)
  {
    return llvm::Type::getDoubleTy(context);
  }
};

}  // namespace llvm_helpers