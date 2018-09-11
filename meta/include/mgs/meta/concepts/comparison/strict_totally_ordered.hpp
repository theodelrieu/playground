#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/comparison/equality_comparable.hpp>
#include <mgs/meta/concepts/core/complete_type.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/greater_or_equal_than.hpp>
#include <mgs/meta/detected/operators/greater_than.hpp>
#include <mgs/meta/detected/operators/less_or_equal_than.hpp>
#include <mgs/meta/detected/operators/less_than.hpp>

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
template <typename T, typename = void>
struct is_strict_totally_ordered_impl : std::false_type
{
};

template <typename T>
struct is_strict_totally_ordered_impl<T, std::enable_if_t<core::is_complete_type<T>::value>>
  : std::integral_constant<
        bool,
        is_equality_comparable<T>::value &&
            is_detected_exact<bool,
                              detected::operators::less_than,
                              T const&,
                              T const&>::value &&
            is_detected_exact<bool,
                              detected::operators::less_or_equal_than,
                              T const&,
                              T const&>::value &&
            is_detected_exact<bool,
                              detected::operators::greater_than,
                              T const&,
                              T const&>::value &&
            is_detected_exact<bool,
                              detected::operators::greater_or_equal_than,
                              T const&,
                              T const&>::value>
{
};
}

template <typename T>
struct is_strict_totally_ordered: detail::is_strict_totally_ordered_impl<T>
{
  using requirements = std::tuple<is_equality_comparable<T>>;
  using const_lvalue_ref = std::add_lvalue_reference_t<std::add_const_t<T>>;

  static constexpr auto const has_less_than =
      is_detected_exact<bool,
                        detected::operators::less_than,
                        const_lvalue_ref,
                        const_lvalue_ref>::value;

  static constexpr auto const has_less_or_equal_than =
      is_detected_exact<bool,
                        detected::operators::less_or_equal_than,
                        const_lvalue_ref,
                        const_lvalue_ref>::value;

  static constexpr auto const has_greater_than =
      is_detected_exact<bool,
                        detected::operators::greater_than,
                        const_lvalue_ref,
                        const_lvalue_ref>::value;

  static constexpr auto const has_greater_or_equal_than =
      is_detected_exact<bool,
                        detected::operators::greater_or_equal_than,
                        const_lvalue_ref,
                        const_lvalue_ref>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(is_strict_totally_ordered::value,
                  "T is not StrictTotallyOrdered");
    static_assert(
        has_less_than,
        "Missing or invalid operator: 'bool operator<(T const&, T const&)'");
    static_assert(
        has_less_or_equal_than,
        "Missing or invalid operator: 'bool operator<=(T const&, T const&)'");
    static_assert(
        has_greater_than,
        "Missing or invalid operator: 'bool operator>(T const&, T const&)'");
    static_assert(
        has_greater_or_equal_than,
        "Missing or invalid operator: 'bool operator>=(T const&, T const&)'");
    return 1;
  }
};

template <typename T,
          typename = std::enable_if_t<is_strict_totally_ordered<T>::value>>
using StrictTotallyOrdered = T;
}
};
}
}
}
