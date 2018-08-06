#pragma once

#include <cstddef>
#include <utility>

namespace mgs
{
inline namespace v1
{
namespace base_n
{
namespace binary_to_text
{
namespace detail
{
template <std::size_t A, std::size_t Multiple>
constexpr std::size_t round_to_multiple_of()
{
  return ((A + (Multiple / 2)) / Multiple) * Multiple;
}

template <std::size_t X, std::size_t N>
constexpr std::size_t pow()
{
  std::size_t ret = 1;
  auto x = X;
  auto n = N;

  while (n > 0)
  {
    if (n & 1)
      ret *= x;
    n >>= 1;
    x *= x;
  }
  return ret;
}
}
}
}
}
}
