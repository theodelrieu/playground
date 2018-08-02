#pragma once

#include <cstddef>
#include <utility>

namespace mgs
{
namespace base_n
{
inline namespace v1
{
namespace detail
{
template <std::size_t A, std::size_t Multiple>
constexpr std::size_t round_to_multiple_of()
{
  return ((A + (Multiple / 2)) / Multiple) * Multiple;
}
}
}
}
}
