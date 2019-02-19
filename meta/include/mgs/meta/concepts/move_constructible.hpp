#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/constructible.hpp>
#include <mgs/meta/concepts/convertible_to.hpp>
#include <mgs/meta/detected.hpp>

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace concepts
{
template <typename T>
struct is_move_constructible
{
  using requirements =
      std::tuple<is_constructible<T, T>, is_convertible_to<T, T>>;

  static constexpr auto const value =
      is_constructible<T, T>::value && is_convertible_to<T, T>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not MoveConstructible");
    return 1;
  }
};
}

template <typename T,
          typename =
              std::enable_if_t<concepts::is_move_constructible<T>::value>>
using MoveConstructible = T;
}
}
}
