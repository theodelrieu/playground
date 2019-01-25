
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
template <typename T, typename TupleArgs, typename = void>
struct is_constructible_impl : std::false_type
{
};

template <typename T, typename ...Args>
using detect_constructible = decltype(T(std::declval<Args>()...));

template <typename T, typename... Args>
struct is_constructible_impl<
    T,
    std::tuple<Args...>,
    std::enable_if_t<(std::is_object<T>::value ||
                      std::is_reference<T>::value) &&
                     is_detected<detect_constructible, T, Args...>::value>>
  : std::true_type
{
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
