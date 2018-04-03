#pragma once

#include <type_traits>

#include <b64/detail/meta/concepts/iterator.hpp>
#include <b64/detail/meta/concepts/sentinel.hpp>
#include <b64/detail/meta/detected.hpp>

// concept Iterable = requires(T& a) {
//  requires { begin(a) } -> Iterator
//  requires { end(a) } -> Sentinel<Iterator>
// }
namespace b64
{
namespace detail
{
namespace detail2
{
using std::begin;
using std::end;

#define CAN_CALL_STD_FUNC_IMPL(name)                             \
  template <typename T>                                          \
  struct can_call_##name##_impl                                  \
  {                                                              \
    template <typename U>                                        \
    static auto check(int)                                       \
        -> decltype(name(std::declval<T&>()), std::true_type()); \
                                                                 \
    template <typename>                                          \
    static std::false_type check(...);                           \
                                                                 \
    using type = decltype(check<T>(0));                          \
  }

CAN_CALL_STD_FUNC_IMPL(begin);
CAN_CALL_STD_FUNC_IMPL(end);

template <typename T>
struct can_call_begin : can_call_begin_impl<T>::type
{
};

template <typename T>
struct can_call_end : can_call_end_impl<T>::type
{
};
}

namespace detail3
{
  // TODO put that in a detail call_std stuff, move stuff from swappable too
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
}
}
