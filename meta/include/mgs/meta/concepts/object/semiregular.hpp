#pragma once

#include <tuple>
#include <type_traits>

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
template <typename T, typename = void>
struct is_semiregular : std::false_type
{
  //FIXME do everything??? is_default_constructible etc??
  using requirements = std::tuple<core::is_swappable<T>>;

  struct static_assert_t
  {
    static_assert(sizeof(T) == 0, "T is not Semiregular");
  };
};

// is_default_constructible fails to compile if T is an incomplete type
template <typename T>
struct is_semiregular<T, std::enable_if_t<sizeof(T) != 0>>
  : std::integral_constant<bool,
  !std::is_same<void, T>::value &&
                           std::is_default_constructible<T>::value &&
                               std::is_copy_constructible<T>::value &&
                               std::is_move_constructible<T>::value &&
                               std::is_copy_assignable<T>::value &&
                               std::is_move_assignable<T>::value &&
                               std::is_destructible<T>::value &&
                               core::is_swappable<T>::value>
{
  using requirements = std::tuple<core::is_swappable<T>>;

  struct static_assert_t
  {
    static_assert(is_semiregular::value, "T is not Semiregular");
  };
};

template <typename T, typename = std::enable_if_t<is_semiregular<T>::value>>
using Semiregular = T;
}
};
}
}
}
