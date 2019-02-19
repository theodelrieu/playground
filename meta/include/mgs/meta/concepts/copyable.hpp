
#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/assignable.hpp>
#include <mgs/meta/concepts/copy_constructible.hpp>
#include <mgs/meta/concepts/movable.hpp>

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace concepts
{
template <typename T>
struct is_copyable
{
  using requirements = std::tuple<
      is_assignable<std::add_lvalue_reference_t<T>,
                    std::add_lvalue_reference_t<std::add_const_t<T>>>,
      is_movable<T>,
      is_copy_constructible<T>>;

  static constexpr auto const value =
      is_assignable<std::add_lvalue_reference_t<T>,
                    std::add_lvalue_reference_t<std::add_const_t<T>>>::value &&
      is_movable<T>::value && is_copy_constructible<T>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not Copyable");
    return 1;
  }
};
}

template <typename T,
          typename = std::enable_if_t<concepts::is_copyable<T>::value>>
using Copyable = T;
}
}
}
