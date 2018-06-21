#pragma once

#include <cstdint>
#include <type_traits>
#include <utility>

#include <mgs/detail/meta/can_call_std.hpp>

// concept Swappable: http://en.cppreference.com/w/cpp/concept/Swappable
namespace mgs
{
CAN_CALL_STD_FUNC_IMPL(swap)

namespace detail
{

// This mess is required since std::swap only got SFINAE-correctness in C++17
template <typename T>
struct is_swappable
  : std::integral_constant<bool,
                           detail2::can_call_swap<T&, T&>::value &&
                               (!detail3::would_call_std_swap<T&, T&>::value ||
                                (std::is_move_assignable<T>::value &&
                                 std::is_move_constructible<T>::value))>
{
};

template <typename T, std::size_t N>
struct is_swappable<T[N]> : is_swappable<T>
{
};
}
}