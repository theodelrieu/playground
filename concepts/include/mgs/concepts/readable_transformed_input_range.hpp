#pragma once

#include <cstddef>
#include <tuple>
#include <type_traits>

#include <mgs/concepts/detail/detected/member_functions/read.hpp>
#include <mgs/concepts/transformed_input_range.hpp>
#include <mgs/meta/concepts/iterator/iterator.hpp>

// clang-format off
// template <typename T, typename O>
// concept ReadableTransformedInputRange =
//   TransformedInputRange<T> &&
//   std::OutputIterator<O, typename T::value_type> &&
//   requires(T& v, O o, std::size_t n) {
//     { v.read(o, n) } -> std::Same<std::size_t>;
// };
// clang-format on

namespace mgs
{
inline namespace v1
{
namespace concepts
{
template <typename T, typename O>
struct is_readable_transformed_input_range
{
private:
  using lvalue_ref = std::add_lvalue_reference_t<T>;

  static auto constexpr const has_read_method =
      meta::is_detected_exact<std::size_t,
                              detail::detected::member_functions::read,
                              lvalue_ref,
                              O,
                              std::size_t>::value;

public:
  using requirements = std::tuple<meta::concepts::iterator::is_iterator<O>,
                                  is_transformed_input_range<T>>;

  static constexpr auto const value =
      is_transformed_input_range<T>::value && has_read_method &&
      meta::concepts::iterator::is_iterator<O>::value;

  constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not a ReadableTransformedInputRange");
    static_assert(has_read_method,
                  "Invalid or missing function: 'std::size_t "
                  "T::read(T::value_type*, std::size_t)'");
    return -1;
  }
};
}

template <typename T,
          typename O,
          typename = std::enable_if_t<
              concepts::is_readable_transformed_input_range<T, O>::value>>
using ReadableTransformedInputRange = T;
}
}
