#pragma once

#include <cstdint>
#include <tuple>
#include <type_traits>
#include <utility>

#include <mgs/meta/call_std/swap.hpp>
#include <mgs/meta/concepts/complete_type.hpp>

// concept Swappable: http://en.cppreference.com/w/cpp/concept/Swappable
namespace mgs
{
namespace meta
{
namespace concepts
{
namespace detail
{
// This mess is required since std::swap only got SFINAE-correctness in C++17
// https://stackoverflow.com/a/26745591
template <typename T, typename = void>
struct is_swappable_impl : std::false_type
{
};

template <typename T>
struct is_swappable_impl<
    T,
    std::enable_if_t<is_complete_type<T>::value && !std::is_array<T>::value>>
  : std::integral_constant<bool,
                           is_detected<detail2::result_of_swap, T&, T&>::value &&
                               (!would_call_std_swap<T&, T&>::value ||
                                (std::is_move_assignable<T>::value &&
                                 std::is_move_constructible<T>::value))>
{
};

template <typename T, std::size_t N>
struct is_swappable_impl<T[N]> : is_swappable_impl<T>
{
};
}

template <typename T>
struct is_swappable : detail::is_swappable_impl<std::remove_reference_t<T>>
{
  using requirements = std::tuple<>;

  static constexpr int trigger_static_asserts()
  {
    static_assert(is_swappable::value, "T is not Swappable");
    return 1;
  }
};

template <typename T>
constexpr auto is_swappable_v = is_swappable<T>::value;
}

template <typename T,
          typename = std::enable_if_t<concepts::is_swappable<T>::value>>
using Swappable = T;
}
}
