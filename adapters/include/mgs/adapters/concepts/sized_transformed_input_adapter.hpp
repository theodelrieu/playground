#pragma once

#include <cstddef>
#include <tuple>

#include <mgs/adapters/concepts/transformed_input_adapter.hpp>
#include <mgs/adapters/detail/detected/member_functions/max_transformed_size.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/types/value_type.hpp>

// clang-format off
//
// template <typename T, OutputIterator OI = typename T::value_type*>
// concept SizedTransformedInputAdapter = TransformedInputAdapter<T, OI> &&
//   requires (T const& v) {
//     { v.max_transformed_size() } -> std::size_t
//   };
//
// clang-format on

namespace mgs
{
inline namespace v1
{
namespace adapters
{
namespace concepts
{
template <typename T,
          typename OutputIterator = std::add_pointer_t<
              meta::detected_t<meta::detected::types::value_type, T>>>
struct is_sized_transformed_input_adapter
{
private:
  using lvalue_const_ref = std::add_lvalue_reference_t<std::add_const_t<T>>;

  static constexpr auto const has_max_transformed_size =
      meta::is_detected_exact<
          std::size_t,
          detail::detected::member_functions::max_transformed_size,
          lvalue_const_ref>::value;

public:
  using requirements =
      std::tuple<is_transformed_input_adapter<T, OutputIterator>>;

  static constexpr auto const value =
      is_transformed_input_adapter<T, OutputIterator>::value &&
      has_max_transformed_size;

  constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not a SizedTransformedInputAdapter");
    static_assert(has_max_transformed_size,
                  "Invalid or missing function: 'std::size_t "
                  "T::max_transformed_size() const'");
    return 1;
  }
};

template <typename T,
          typename OutputIterator = std::add_pointer_t<
              meta::detected_t<meta::detected::types::value_type, T>>,
          typename = std::enable_if_t<
              is_sized_transformed_input_adapter<T, OutputIterator>::value>>
using SizedTransformedInputAdapter = T;
}
}
}
}
