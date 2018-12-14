#pragma once

#include <cstddef>
#include <tuple>

#include <mgs/adapters/concepts/transformed_input_adapter.hpp>
#include <mgs/adapters/detail/detected/member_functions/max_transformed_size.hpp>
#include <mgs/meta/detected.hpp>

// template <typename T>
// concept SizedTransformedInputAdapter = TransformedInputAdapter<T> &&
//   requires (T const& v) {
//     { v.max_transformed_size() } -> std::size_t
//   }

namespace mgs
{
inline namespace v1
{
namespace adapters
{
namespace concepts
{
template <typename T>
struct is_sized_transformed_input_adapter
{
private:
  static constexpr auto const has_max_transformed_size = meta::is_detected_exact<
      std::size_t,
      detail::detected::member_functions::max_transformed_size,
      T const&>::value;

public:
  using requirements = std::tuple<is_transformed_input_adapter<T>>;

  static constexpr auto const value =
      is_transformed_input_adapter<T>::value && has_max_transformed_size;

  constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not a SizedTransformedInputAdapter");
    static_assert(has_max_transformed_size,
                  "Missing or invalid function: 'std::size_t "
                  "T::max_transformed_size() const'");
    return 1;
  }
};

template <typename T,
          typename =
              std::enable_if_t<is_sized_transformed_input_adapter<T>::value>>
using SizedTransformedInputAdapter = T;
}
}
}
}
