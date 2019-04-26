#pragma once

#include <cstddef>
#include <tuple>

#include <mgs/concepts/detected/member_functions/max_transformed_size.hpp>
#include <mgs/concepts/transformed_input_range.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/types/value_type.hpp>
#include <mgs/meta/ssize_t.hpp>

// clang-format off
//
// template <typename T>
// concept SizedTransformedInputRange = TransformedInputRange<T> &&
//   requires (T const& v) {
//     { v.max_transformed_size() } -> meta::ssize_t
//   };
//
// clang-format on

namespace mgs
{
template <typename T>
struct is_sized_transformed_input_range
{
private:
  using lvalue_const_ref = std::add_lvalue_reference_t<std::add_const_t<T>>;

  static constexpr auto const has_max_transformed_size =
      meta::is_detected_convertible<
          meta::ssize_t,
          detected::member_functions::max_transformed_size,
          lvalue_const_ref>::value;

public:
  using requirements = std::tuple<is_transformed_input_range<T>>;

  static constexpr auto const value =
      is_transformed_input_range<T>::value && has_max_transformed_size;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not a SizedTransformedInputRange");
    static_assert(has_max_transformed_size,
                  "Invalid or missing function: 'meta::ssize_t "
                  "T::max_transformed_size() const'");
    return 1;
  }
};

template <typename T>
constexpr auto is_sized_transformed_input_range_v =
    is_sized_transformed_input_range<T>::value;

template <typename T,
          typename = std::enable_if_t<
              is_sized_transformed_input_range<T>::value>>
using SizedTransformedInputRange = T;
}
