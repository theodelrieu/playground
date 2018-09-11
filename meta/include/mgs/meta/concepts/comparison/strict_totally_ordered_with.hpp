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
template <typename T, typename U>
struct is_strict_totally_ordered_with
{
  using requirements =
      std::tuple<is_strict_totally_ordered<T>, is_strict_totally_ordered<U>>;
  using t_const_lvalue_ref = std::add_lvalue_reference_t<std::add_const_t<T>>;
  using u_const_lvalue_ref = std::add_lvalue_reference_t<std::add_const_t<U>>;

  static constexpr auto const has_less_than_t_u =
      is_detected_exact<bool,
                        detected::operators::less_than,
                        t_const_lvalue_ref,
                        u_const_lvalue_ref>::value;

  static constexpr auto const has_less_than_u_t =
      is_detected_exact<bool,
                        detected::operators::less_than,
                        u_const_lvalue_ref,
                        t_const_lvalue_ref>::value;

  static constexpr auto const has_less_or_equal_than_t_u =
      is_detected_exact<bool,
                        detected::operators::less_or_equal_than,
                        t_const_lvalue_ref,
                        u_const_lvalue_ref>::value;

  static constexpr auto const has_less_or_equal_than_u_t =
      is_detected_exact<bool,
                        detected::operators::less_or_equal_than,
                        u_const_lvalue_ref,
                        t_const_lvalue_ref>::value;

  static constexpr auto const has_greater_than_t_u =
      is_detected_exact<bool,
                        detected::operators::greater_than,
                        t_const_lvalue_ref,
                        u_const_lvalue_ref>::value;

  static constexpr auto const has_greater_than_u_t =
      is_detected_exact<bool,
                        detected::operators::greater_than,
                        u_const_lvalue_ref,
                        t_const_lvalue_ref>::value;

  static constexpr auto const has_greater_or_equal_than_t_u =
      is_detected_exact<bool,
                        detected::operators::greater_or_equal_than,
                        t_const_lvalue_ref,
                        u_const_lvalue_ref>::value;

  static constexpr auto const has_greater_or_equal_than_u_t =
      is_detected_exact<bool,
                        detected::operators::greater_or_equal_than,
                        u_const_lvalue_ref,
                        t_const_lvalue_ref>::value;

  static constexpr auto const value =
      is_strict_totally_ordered<T>::value &&
      is_strict_totally_ordered<U>::value && has_less_than_t_u &&
      has_less_than_u_t && has_less_or_equal_than_t_u &&
      has_less_or_equal_than_u_t && has_greater_than_t_u &&
      has_greater_than_u_t && has_greater_or_equal_than_t_u &&
      has_greater_or_equal_than_u_t;

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
    return 1;
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
