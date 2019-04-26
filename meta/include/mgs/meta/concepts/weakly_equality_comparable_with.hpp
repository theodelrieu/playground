#pragma once

#include <tuple>
#include <type_traits>
#include <utility>

#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/equality.hpp>
#include <mgs/meta/detected/operators/inequality.hpp>

// http://en.cppreference.com/w/cpp/experimental/ranges/concepts/WeaklyEqualityComparableWith
namespace mgs
{
namespace meta
{
template <typename T, typename U>
struct is_weakly_equality_comparable_with
{
private:
  using t_const_lvalue_ref = std::add_lvalue_reference_t<std::add_const_t<T>>;
  using u_const_lvalue_ref = std::add_lvalue_reference_t<std::add_const_t<U>>;

  static constexpr auto const has_equality_t_u =
      meta::is_detected_convertible<bool,
                                    detected::operators::equality,
                                    t_const_lvalue_ref,
                                    u_const_lvalue_ref>::value;

  static constexpr auto const has_equality_u_t =
      meta::is_detected_convertible<bool,
                                    detected::operators::equality,
                                    u_const_lvalue_ref,
                                    t_const_lvalue_ref>::value;

  static constexpr auto const has_inequality_t_u =
      meta::is_detected_convertible<bool,
                                    detected::operators::inequality,
                                    t_const_lvalue_ref,
                                    u_const_lvalue_ref>::value;

  static constexpr auto const has_inequality_u_t =
      meta::is_detected_convertible<bool,
                                    detected::operators::inequality,
                                    u_const_lvalue_ref,
                                    t_const_lvalue_ref>::value;

public:
  using requirements = std::tuple<>;

  static constexpr auto const value = has_equality_t_u && has_equality_u_t &&
                                      has_inequality_t_u && has_inequality_u_t;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not WeaklyEqualityComparable with U");
    static_assert(
        has_equality_t_u,
        "Invalid or missing operator: 'bool operator==(T const&, U const&)'");
    static_assert(
        has_equality_u_t,
        "Invalid or missing operator: 'bool operator==(U const&, T const&)'");
    static_assert(
        has_inequality_t_u,
        "Invalid or missing operator: 'bool operator!=(T const&, U const&)'");
    static_assert(
        has_inequality_u_t,
        "Invalid or missing operator: 'bool operator!=(U const&, T const&)'");
    return 1;
  }
};

template <typename T, typename U>
constexpr auto is_weakly_equality_comparable_with_v =
    is_weakly_equality_comparable_with<T, U>::value;

template <typename T,
          typename U,
          typename =
              std::enable_if_t<is_weakly_equality_comparable_with<T, U>::value>>
using WeaklyEqualityComparableWith = T;
}
}
