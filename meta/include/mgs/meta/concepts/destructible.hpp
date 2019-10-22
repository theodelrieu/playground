#pragma once

#include <tuple>
#include <type_traits>

namespace mgs
{
namespace meta
{
template <typename T>
struct is_destructible
{
  using requirements = std::tuple<>;

  static constexpr auto const value = std::is_nothrow_destructible<T>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not destructible");
    return 1;
  }
};

template <typename T>
constexpr auto is_destructible_v = is_destructible<T>::value;

template <typename T, typename = std::enable_if_t<is_destructible<T>::value>>
using destructible = T;
}
}
