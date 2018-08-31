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
template <typename T>
struct is_swappable
  : std::integral_constant<bool,
                           can_call_swap<T&, T&>::value &&
                               (!would_call_std_swap<T&, T&>::value ||
                                (std::is_move_assignable<T>::value &&
                                 std::is_move_constructible<T>::value))>
{
  //FIXME
  using requirements = std::tuple<>;
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
