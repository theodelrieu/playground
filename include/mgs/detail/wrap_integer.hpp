#pragma once

#include <cstdlib>

#include <type_traits>

namespace mgs
{
namespace detail
{
template <int Low, int High, typename = std::enable_if<(Low < High)>>
constexpr int wrap_integer(int n)
{
  static_assert(Low < High, "");
  auto constexpr nb_values = High - Low + 1;
  auto const m = (n - Low) % (High + 1 - Low);
  if (m < 0)
    return m + nb_values + Low;
  return m + Low;
}
}
}
