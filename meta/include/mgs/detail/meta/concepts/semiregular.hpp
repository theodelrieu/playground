#pragma once

#include <type_traits>

#include <mgs/detail/meta/concepts/swappable.hpp>

namespace mgs
{
namespace detail
{
template <typename T, typename = void>
struct is_semiregular
  : std::integral_constant<bool,
                           std::is_default_constructible<T>::value &&
                               std::is_copy_constructible<T>::value &&
                               std::is_move_constructible<T>::value &&
                               std::is_copy_assignable<T>::value &&
                               std::is_move_assignable<T>::value &&
                               std::is_destructible<T>::value &&
                               is_swappable<T>::value>
{
};
}
}
