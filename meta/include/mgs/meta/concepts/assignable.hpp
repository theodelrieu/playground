#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/detected.hpp>
#include <mgs/meta/concepts/common_reference.hpp>

// https://en.cppreference.com/w/cpp/concepts/Assignable

namespace mgs
{
namespace detail
{
template <typename LHS, typename RHS>
auto _assignable_requirement(LHS lhs, RHS&& rhs)
    -> decltype(lhs = std::forward<RHS>(rhs));

template <typename LHS, typename RHS>
using assignable_requirement =
    decltype(_assignable_requirement(std::declval<LHS>(), std::declval<RHS>()));
}

namespace meta
{
template <typename LHS, typename RHS>
struct is_assignable
{
private:
  static constexpr auto const does_return_same_type =
      is_detected_exact<LHS, detail::assignable_requirement, LHS, RHS>::value;

  using lhs_lvalue_const_ref = std::add_lvalue_reference_t<
      std::add_const_t<std::remove_reference_t<LHS>>>;

  using rhs_lvalue_const_ref = std::add_lvalue_reference_t<
      std::add_const_t<std::remove_reference_t<RHS>>>;

public:
  using requirements = std::tuple<
      has_common_reference<lhs_lvalue_const_ref, rhs_lvalue_const_ref>>;

  static constexpr auto const value =
      does_return_same_type &&
      has_common_reference<lhs_lvalue_const_ref, rhs_lvalue_const_ref>::value &&
      std::is_lvalue_reference<LHS>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "LHS is not assignable from RHS");
    static_assert(std::is_lvalue_reference<LHS>::value,
                  "LHS must be an lvalue reference");
    static_assert(does_return_same_type, "assignment operator must return LHS");
    return 1;
  }
};

template <typename LHS, typename RHS>
constexpr auto is_assignable_v = is_assignable<LHS, RHS>::value;

template <typename LHS,
          typename RHS,
          typename = std::enable_if_t<is_assignable<LHS, RHS>::value>>
using Assignable = LHS;
}
}
