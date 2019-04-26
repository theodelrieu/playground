#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/swappable.hpp>
#include <mgs/meta/concepts/complete_type.hpp>

namespace mgs
{
namespace detail
{
template <typename T, typename = void>
struct is_semiregular_impl : std::false_type
{
  static constexpr int trigger_static_asserts()
  {
    return 1;
  }
};

template <typename T>
struct is_semiregular_impl<T,
                           std::enable_if_t<meta::is_complete_type<T>::value>>
  : std::integral_constant<bool,
                           std::is_default_constructible<T>::value &&
                               std::is_copy_constructible<T>::value &&
                               std::is_move_constructible<T>::value &&
                               std::is_copy_assignable<T>::value &&
                               std::is_move_assignable<T>::value &&
                               std::is_destructible<T>::value &&
                               meta::is_swappable<T>::value>
{
private:
  static constexpr auto const is_default_constructible =
      std::is_default_constructible<T>::value;

  static constexpr auto const is_copy_constructible =
      std::is_copy_constructible<T>::value;

  static constexpr auto const is_move_constructible =
      std::is_move_constructible<T>::value;

  static constexpr auto const is_copy_assignable =
      std::is_copy_assignable<T>::value;

  static constexpr auto const is_move_assignable =
      std::is_move_assignable<T>::value;

  static constexpr auto const is_destructible = std::is_destructible<T>::value;

public:
  static constexpr int trigger_static_asserts()
  {
    static_assert(is_default_constructible, "T is not default constructible");
    static_assert(is_copy_constructible, "T is not copy constructible");
    static_assert(is_move_constructible, "T is not move constructible");
    static_assert(is_copy_assignable, "T is not copy assignable");
    static_assert(is_move_assignable, "T is not move assignable");
    static_assert(is_destructible, "T is not destructible");
    return 1;
  }
};
}

namespace meta
{
template <typename T>
struct is_semiregular : detail::is_semiregular_impl<T>
{
  using requirements = std::tuple<is_swappable<T>>;

  static constexpr int trigger_static_asserts()
  {
    static_assert(is_semiregular::value, "T is not Semiregular");
    return detail::is_semiregular_impl<T>::trigger_static_asserts();
  }
};

template <typename T>
constexpr auto is_semiregular_v = is_semiregular<T>::value;

template <typename T, typename = std::enable_if_t<is_semiregular<T>::value>>
using Semiregular = T;
}
}
