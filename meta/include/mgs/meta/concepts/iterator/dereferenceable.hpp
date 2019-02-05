#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/detected/operators/dereference.hpp>
#include <mgs/meta/void_t.hpp>

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace concepts
{
namespace iterator
{
namespace detail
{
template <typename T>
using with_reference = T&;

template <typename T, typename = void>
struct can_reference : std::false_type
{
};

template <typename T>
struct can_reference<T, void_t<with_reference<T>>> : std::true_type
{
};

template <typename T, typename = void>
struct is_dereferenceable_impl : std::false_type
{
};

template <typename T>
struct is_dereferenceable_impl<
    T,
    std::enable_if_t<is_detected<detected::operators::dereference, T>::value>>
{
  static constexpr auto const value =
      can_reference<detected::operators::dereference<T>>::value;
};
}

template <typename T>
struct is_dereferenceable: detail::is_dereferenceable_impl<T>
{
  using requirements = std::tuple<>;

  static constexpr int trigger_static_asserts()
  {
    static_assert(is_dereferenceable::value, "T is not dereferenceable");
    return 1;
  }
};

template <typename T, typename = std::enable_if_t<is_dereferenceable<T>::value>>
using Dereferenceable = T;
}
}
}
}
}
