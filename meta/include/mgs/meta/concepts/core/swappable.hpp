#pragma once

#include <cstdint>
#include <tuple>
#include <type_traits>
#include <utility>

#include <mgs/meta/call_std/swap.hpp>
#include <mgs/meta/concepts/core/complete_type.hpp>

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
                           can_call_swap<T&, T&>::value &&
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

  struct static_assert_t
  {
    static constexpr int trigger()
    {
      static_assert(is_swappable::value, "T is not Swappable");
      return 1;
    }
  };
};

template <typename T, typename = std::enable_if_t<is_swappable<T>::value>>
using Swappable = T;
}
};
}
}
}
