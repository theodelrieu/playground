#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/equality_comparable.hpp>
#include <mgs/meta/concepts/complete_type.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/greater_or_equal_than.hpp>
#include <mgs/meta/detected/operators/greater_than.hpp>
#include <mgs/meta/detected/operators/less_or_equal_than.hpp>
#include <mgs/meta/detected/operators/less_than.hpp>

namespace mgs
{
namespace meta
{
template <typename T>
struct is_strict_totally_ordered
{
private:
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
public:
  using requirements = std::tuple<is_equality_comparable<T>>;

  static constexpr auto const value =
      is_equality_comparable<T>::value && has_less_than && has_greater_than &&
      has_less_or_equal_than && has_greater_or_equal_than;

  static constexpr int trigger_static_asserts()
  {
    static_assert(is_strict_totally_ordered::value,
                  "T is not StrictTotallyOrdered");
    static_assert(
        has_less_than,
        "Invalid or missing operator: 'bool operator<(T const&, T const&)'");
    static_assert(
        has_less_or_equal_than,
        "Invalid or missing operator: 'bool operator<=(T const&, T const&)'");
    static_assert(
        has_greater_than,
        "Invalid or missing operator: 'bool operator>(T const&, T const&)'");
    static_assert(
        has_greater_or_equal_than,
        "Invalid or missing operator: 'bool operator>=(T const&, T const&)'");
    return 1;
  }
};

template <typename T>
constexpr auto is_strict_totally_ordered_v =
    is_strict_totally_ordered<T>::value;

template <typename T,
          typename = std::enable_if_t<is_strict_totally_ordered<T>::value>>
using StrictTotallyOrdered = T;
}
}
