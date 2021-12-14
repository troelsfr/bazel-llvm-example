#pragma once
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

namespace compiler {

namespace details {
template <typename T>
struct SizeOf
{
  constexpr static uint64_t value()
  {
    return sizeof(T);
  }
};

template <>
struct SizeOf<void>
{
  constexpr static uint64_t value()
  {
    return 0;
  }
};

}  // namespace details

}  // namespace compiler