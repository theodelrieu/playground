#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/comparison/strict_totally_ordered.hpp>
#include <mgs/meta/concepts/core/complete_type.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/greater_or_equal_than.hpp>
#include <mgs/meta/detected/operators/greater_than.hpp>
#include <mgs/meta/detected/operators/less_or_equal_than.hpp>
#include <mgs/meta/detected/operators/less_than.hpp>

// http://en.cppreference.com/w/cpp/experimental/ranges/concepts/StrictTotallyOrdered

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace concepts
{
namespace comparison
{
namespace detail
{
template <typename T, typename U, typename = void>
struct is_strict_totally_ordered_with_impl : std::false_type
{
};

template <typename T, typename U>
struct is_strict_totally_ordered_with_impl<
    T,
    U,
    std::enable_if_t<core::is_complete_type<T>::value &&
                     core::is_complete_type<U>::value>>
  : std::integral_constant<
        bool,
        is_strict_totally_ordered<T>::value &&
            is_strict_totally_ordered<U>::value &&
            is_detected_exact<bool,
                              detected::operators::less_than,
                              T const&,
                              U const&>::value &&
            is_detected_exact<bool,
                              detected::operators::less_or_equal_than,
                              T const&,
                              U const&>::value &&
            is_detected_exact<bool,
                              detected::operators::greater_than,
                              T const&,
                              U const&>::value &&
            is_detected_exact<bool,
                              detected::operators::greater_or_equal_than,
                              T const&,
                              U const&>::value &&
            is_detected_exact<bool,
                              detected::operators::less_than,
                              U const&,
                              T const&>::value &&
            is_detected_exact<bool,
                              detected::operators::less_or_equal_than,
                              U const&,
                              T const&>::value &&
            is_detected_exact<bool,
                              detected::operators::greater_than,
                              U const&,
                              T const&>::value &&
            is_detected_exact<bool,
                              detected::operators::greater_or_equal_than,
                              U const&,
                              T const&>::value>
{
};
}

template <typename T, typename U>
struct is_strict_totally_ordered_with
  : detail::is_strict_totally_ordered_with_impl<T, U>
{
  using requirements =
      std::tuple<is_strict_totally_ordered<T>, is_strict_totally_ordered<U>>;

  struct static_assert_t
  {
    static constexpr auto const has_less_than_t_u =
        is_detected_exact<bool,
                          detected::operators::less_than,
                          T const&,
                          U const&>::value;

    static constexpr auto const has_less_than_u_t =
        is_detected_exact<bool,
                          detected::operators::less_than,
                          U const&,
                          T const&>::value;

    static constexpr auto const has_less_or_equal_than_t_u =
        is_detected_exact<bool,
                          detected::operators::less_or_equal_than,
                          T const&,
                          U const&>::value;

    static constexpr auto const has_less_or_equal_than_u_t =
        is_detected_exact<bool,
                          detected::operators::less_or_equal_than,
                          U const&,
                          T const&>::value;

    static constexpr auto const has_greater_than_t_u =
        is_detected_exact<bool,
                          detected::operators::greater_than,
                          T const&,
                          U const&>::value;

    static constexpr auto const has_greater_than_u_t =
        is_detected_exact<bool,
                          detected::operators::greater_than,
                          U const&,
                          T const&>::value;

    static constexpr auto const has_greater_or_equal_than_t_u =
        is_detected_exact<bool,
                          detected::operators::greater_or_equal_than,
                          T const&,
                          U const&>::value;

    static constexpr auto const has_greater_or_equal_than_u_t =
        is_detected_exact<bool,
                          detected::operators::greater_or_equal_than,
                          U const&,
                          T const&>::value;

    static constexpr int trigger_static_asserts()
    {
      static_assert(is_strict_totally_ordered_with::value,
                    "T is not StrictTotallyOrderedWith U");

      static_assert(
          has_less_than_t_u,
          "Missing or invalid operator: 'bool operator<(T const&, U const&)'");
      static_assert(
          has_less_than_u_t,
          "Missing or invalid operator: 'bool operator<(U const&, T const&)'");
      static_assert(
          has_less_or_equal_than_t_u,
          "Missing or invalid operator: 'bool operator<=(T const&, U const&)'");
      static_assert(
          has_less_or_equal_than_u_t,
          "Missing or invalid operator: 'bool operator<=(U const&, T const&)'");
      static_assert(
          has_greater_than_t_u,
          "Missing or invalid operator: 'bool operator>(T const&, U const&)'");
      static_assert(
          has_greater_than_u_t,
          "Missing or invalid operator: 'bool operator>(U const&, T const&)'");
      static_assert(
          has_greater_or_equal_than_t_u,
          "Missing or invalid operator: 'bool operator>=(T const&, U const&)'");
      static_assert(
          has_greater_or_equal_than_t_u,
          "Missing or invalid operator: 'bool operator>=(U const&, T const&)'");
      return 0;
    };

    static constexpr auto _ = trigger_static_asserts();
  };
};

template <typename T,
          typename U,
          typename =
              std::enable_if_t<is_strict_totally_ordered_with<T, U>::value>>
using StrictTotallyOrderedWith = T;
}
};
}
}
}
