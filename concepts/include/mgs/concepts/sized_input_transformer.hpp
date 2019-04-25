#pragma once

#include <cstddef>
#include <tuple>
#include <type_traits>

#include <mgs/concepts/detail/detected/member_functions/max_transformed_size.hpp>
#include <mgs/concepts/input_transformer.hpp>
#include <mgs/meta/ssize_t.hpp>

// template <typename T>
// concept SizedInputTransformer = InputTransformer<T> &&
//   requires(T const& v) {
//     { v.max_transformed_size() } -> meta::ssize_t;
//   }

namespace mgs
{
namespace concepts
{
template <typename T>
struct is_sized_input_transformer
{
private:
  static constexpr auto const has_max_transformed_size_method =
      meta::is_detected_convertible<
          meta::ssize_t,
          detail::detected::member_functions::max_transformed_size,
          std::add_lvalue_reference_t<std::add_const_t<T>>>::value;

public:
  using requirements = std::tuple<is_input_transformer<T>>;

  static constexpr auto const value =
      is_input_transformer<T>::value && has_max_transformed_size_method;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not an SizedInputTransformer");
    static_assert(has_max_transformed_size_method,
                  "Invalid or missing function: 'meta::ssize_t "
                  "T::max_transformed_size() const'");
    return 1;
  }
};

template <typename T>
constexpr auto is_sized_input_transformer_v =
    is_sized_input_transformer<T>::value;
}

template <typename T,
          typename =
              std::enable_if_t<concepts::is_sized_input_transformer<T>::value>>
using SizedInputTransformer = T;
}
