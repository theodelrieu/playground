
#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/core/assignable.hpp>
#include <mgs/meta/concepts/core/move_constructible.hpp>
#include <mgs/meta/concepts/core/swappable.hpp>

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace concepts
{
namespace object
{
template <typename T>
struct is_movable
{
  using requirements =
      std::tuple<core::is_assignable<std::add_lvalue_reference_t<T>, T>,
                 core::is_move_constructible<T>,
                 core::is_swappable<T>>;

  static constexpr auto const value =
      std::is_object<T>::value &&
      core::is_assignable<std::add_lvalue_reference_t<T>, T>::value &&
      core::is_move_constructible<T>::value && core::is_swappable<T>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not Movable");
    static_assert(std::is_object<T>::value, "T must be an object type");
    return 1;
  }
};

template <typename T, typename = std::enable_if_t<is_movable<T>::value>>
using Movable = T;
}
}
}
}
}
