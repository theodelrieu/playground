#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/common_reference.hpp>
#include <mgs/meta/concepts/common_reference.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/iter_reference_t.hpp>
#include <mgs/meta/iter_rvalue_reference_t.hpp>
#include <mgs/meta/iter_value_t.hpp>
#include <mgs/meta/void_t.hpp>

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace concepts
{
namespace detail
{
template <typename T, typename = void>
struct is_readable_impl : std::false_type
{
  using requirements = std::tuple<>;

  static constexpr int trigger_static_asserts()
  {
    static_assert(is_detected<iter_value_t, T>::value,
                  "iter_value_t<T> is ill-formed");
    static_assert(is_detected<iter_reference_t, T>::value,
                  "iter_reference_t<T> is ill-formed");
    static_assert(is_detected<iter_rvalue_reference_t, T>::value,
                  "iter_rvalue_reference_t<T> is ill-formed");
    return 1;
  }
};

template <typename T>
struct is_readable_impl<
    T,
    void_t<iter_value_t<T>, iter_reference_t<T>, iter_rvalue_reference_t<T>>>
{
  using requirements = std::tuple<
      has_common_reference<
          std::add_rvalue_reference_t<iter_reference_t<T>>,
          std::add_lvalue_reference_t<iter_value_t<T>>>,
      has_common_reference<
          std::add_rvalue_reference_t<iter_reference_t<T>>,
          std::add_rvalue_reference_t<iter_rvalue_reference_t<T>>>,
      has_common_reference<
          std::add_rvalue_reference_t<iter_rvalue_reference_t<T>>,
          std::add_lvalue_reference_t<std::add_const_t<iter_value_t<T>>>>>;

  static constexpr auto const value =
      has_common_reference<
          std::add_rvalue_reference_t<iter_reference_t<T>>,
          std::add_lvalue_reference_t<iter_value_t<T>>>::value &&
      has_common_reference<
          std::add_rvalue_reference_t<iter_reference_t<T>>,
          std::add_rvalue_reference_t<iter_rvalue_reference_t<T>>>::value &&
      has_common_reference<
          std::add_rvalue_reference_t<iter_rvalue_reference_t<T>>,
          std::add_lvalue_reference_t<std::add_const_t<iter_value_t<T>>>>::
          value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not Readable");
    return 1;
  }
};
}

template <typename T>
struct is_readable : detail::is_readable_impl<T>
{
};
}

template <typename T,
          typename = std::enable_if_t<concepts::is_readable<T>::value>>
using Readable = T;
}
}
}
