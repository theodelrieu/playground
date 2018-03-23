#pragma once

#include <cstdint>
#include <type_traits>
#include <utility>

// concept Swappable: http://en.cppreference.com/w/cpp/concept/Swappable
namespace b64
{
namespace detail
{
// source: https://stackoverflow.com/a/26745591
namespace detail2
{
using std::swap;

template <typename T>
struct can_call_swap_impl
{

  template <typename U>
  static auto check(int)
      -> decltype(swap(std::declval<T&>(), std::declval<T&>()),
                  std::true_type());

  template <typename>
  static std::false_type check(...);

  using type = decltype(check<T>(0));
};

template <typename T>
struct can_call_swap : can_call_swap_impl<T>::type
{
};
}

namespace detail3
{
struct tag
{
};

template <class T>
tag swap(T&, T&);

template <typename T>
struct would_call_std_swap_impl
{
  template <typename U>
  static auto check(int) -> std::integral_constant<
      bool,
      std::is_same<decltype(swap(std::declval<U&>(), std::declval<U&>())),
                   tag>::value>;

  template <typename>
  static std::false_type check(...);

  using type = decltype(check<T>(0));
};

template <typename T>
struct would_call_std_swap : would_call_std_swap_impl<T>::type
{
};
}

// This mess is required since std::swap only got SFINAE-correctness in C++17
template <typename T>
struct is_swappable
  : std::integral_constant<bool,
                           detail2::can_call_swap<T>::value &&
                               (!detail3::would_call_std_swap<T>::value ||
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
