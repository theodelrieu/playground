
#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/core/assignable.hpp>
#include <mgs/meta/concepts/core/copy_constructible.hpp>
#include <mgs/meta/concepts/object/movable.hpp>

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
struct is_copyable
{
  using requirements = std::tuple<
      core::is_assignable<std::add_lvalue_reference_t<T>,
                          std::add_lvalue_reference_t<std::add_const_t<T>>>,
      object::is_movable<T>,
      core::is_copy_constructible<T>>;

  static constexpr auto const value =
      core::is_assignable<
          std::add_lvalue_reference_t<T>,
          std::add_lvalue_reference_t<std::add_const_t<T>>>::value &&
      object::is_movable<T>::value && core::is_copy_constructible<T>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not Copyable");
    return 1;
  }
};

template <typename T, typename = std::enable_if_t<is_copyable<T>::value>>
using Copyable = T;
}
}
}
}
}
