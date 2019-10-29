#pragma once

#include <cstddef>
#include <tuple>
#include <type_traits>

#include <mgs/codecs/concepts/input_source.hpp>
#include <mgs/codecs/detected/member_functions/max_remaining_size.hpp>
#include <mgs/meta/concepts/convertible_to.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/ssize_t.hpp>

namespace mgs
{
namespace codecs
{
template <typename T>
struct is_sized_input_source
{
private:
  static constexpr auto const has_max_remaining_size_method =
      meta::is_convertible_to<
          meta::detected_t<detected::member_functions::max_remaining_size,
                           std::add_lvalue_reference_t<std::add_const_t<T>>>,
          meta::ssize_t>::value;

public:
  using requirements = std::tuple<is_input_source<T>>;

  static constexpr auto const value =
      is_input_source<T>::value && has_max_remaining_size_method;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T does not model codecs::sized_input_source");
    static_assert(
        has_max_remaining_size_method,
        "invalid or missing function: 'meta::convertible_to<mgs::ssize_t> "
        "T::max_remaining_size() const'");
    return 1;
  }
};

template <typename T>
constexpr auto is_sized_input_source_v = is_sized_input_source<T>::value;

template <typename T,
          typename = std::enable_if_t<is_sized_input_source<T>::value>>
using sized_input_source = T;
}
}
