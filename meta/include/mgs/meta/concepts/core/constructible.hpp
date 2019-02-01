#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/core/complete_type.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/void_t.hpp>

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace concepts
{
namespace core
{
namespace detail
{
template <typename T>
struct is_valid_is_constructible_type
{
  static constexpr auto const value =
      (std::is_object<T>::value && core::is_complete_type<T>::value) ||
      std::is_array<T>::value || std::is_reference<T>::value ||
      std::is_void<T>::value;
};

template <typename T, typename TupleArgs, typename = void>
struct is_constructible_impl : std::false_type
{
};

template <typename T, typename... Args>
struct is_constructible_impl<
    T,
    std::tuple<Args...>,
    meta::void_t<
        std::enable_if_t<is_valid_is_constructible_type<T>::value>,
        std::enable_if_t<is_valid_is_constructible_type<Args>::value>...>>
{
  static constexpr auto const value = std::is_constructible<T, Args...>::value;
};
}

template <typename T, typename... Args>
struct is_constructible : detail::is_constructible_impl<T, std::tuple<Args...>>
{
  using requirements = std::tuple<>;

  static constexpr int trigger_static_asserts()
  {
    static_assert(is_constructible::value, "T is not constructible from Args...");
    return 1;
  }
};
}
}
}
}
}
