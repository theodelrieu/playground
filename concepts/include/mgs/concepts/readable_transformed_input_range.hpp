#pragma once

#include <cstddef>
#include <tuple>
#include <type_traits>

#include <mgs/concepts/detected/member_functions/read.hpp>
#include <mgs/concepts/transformed_input_range.hpp>
#include <mgs/meta/concepts/output_iterator.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/range_value_t.hpp>
#include <mgs/meta/ssize_t.hpp>

// clang-format off
// template <typename T, typename O>
// concept ReadableTransformedInputRange =
//   TransformedInputRange<T> &&
//   std::OutputIterator<O, std::ranges::range_value_t<T>> &&
//   requires(T& v, O o, meta::ssize_t n) {
//     { v.read(o, n) } -> meta::ssize_t;
// };
// clang-format on

namespace mgs
{
namespace concepts
{
template <typename T, typename O>
struct is_readable_transformed_input_range
{
private:
  using lvalue_ref = std::add_lvalue_reference_t<T>;
  using value_type = meta::detected_t<meta::range_value_t, T>;

  static auto constexpr const has_read_method =
      meta::is_detected_convertible<meta::ssize_t,
                                    detected::member_functions::read,
                                    lvalue_ref,
                                    O,
                                    meta::ssize_t>::value;

public:
  using requirements =
      std::tuple<meta::concepts::is_output_iterator<O, value_type>,
                 is_transformed_input_range<T>>;

  static constexpr auto const value =
      is_transformed_input_range<T>::value && has_read_method &&
      meta::concepts::is_output_iterator<O, value_type>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not a ReadableTransformedInputRange");
    static_assert(has_read_method,
                  "Invalid or missing function: 'meta::ssize_t "
                  "T::read(O, meta::ssize_t)'");
    return -1;
  }
};

template <typename T, typename O>
constexpr auto is_readable_transformed_input_range_v =
    is_readable_transformed_input_range<T, O>::value;
}

template <typename T,
          typename O,
          typename = std::enable_if_t<
              concepts::is_readable_transformed_input_range<T, O>::value>>
using ReadableTransformedInputRange = T;
}
