#pragma once

#include <cstdint>
#include <tuple>
#include <type_traits>
#include <utility>

#include <mgs/meta/call_std/swap.hpp>

// concept Swappable: http://en.cppreference.com/w/cpp/concept/Swappable
namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace concepts
{
namespace core
{
// This mess is required since std::swap only got SFINAE-correctness in C++17
template <typename T, typename = void>
struct is_swappable: std::false_type{
  // FIXME
  using requirements = std::tuple<>;

  struct static_assert_t
  {
    static_assert(sizeof(T) == 0, "T is not Swappable");
  };
};

template <typename T>
struct is_swappable<T, std::enable_if_t<sizeof(T&) != 0>>
  : std::integral_constant<bool,
                           can_call_swap<T&, T&>::value &&
                               (!would_call_std_swap<T&, T&>::value ||
                                (std::is_move_assignable<T>::value &&
                                 std::is_move_constructible<T>::value))>
{
  //FIXME
  using requirements = std::tuple<>;

  struct static_assert_t
  {
    static_assert(is_swappable::value, "T is not Swappable");
  };
};

template <typename T, std::size_t N>
struct is_swappable<T[N]> : is_swappable<T>
{
};

template <typename T, typename = std::enable_if_t<is_swappable<T>::value>>
using Swappable = T;
}
};
}
}
}
