#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/core/swappable.hpp>
#include <mgs/meta/concepts/core/complete_type.hpp>

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace concepts
{
namespace object
{
namespace detail
{
template <typename T, typename = void>
struct is_semiregular_impl : std::false_type
{
};

template <typename T>
struct is_semiregular_impl<T, std::enable_if_t<core::is_complete_type<T>::value>>
  : std::integral_constant<bool,
                           std::is_default_constructible<T>::value &&
                               std::is_copy_constructible<T>::value &&
                               std::is_move_constructible<T>::value &&
                               std::is_copy_assignable<T>::value &&
                               std::is_move_assignable<T>::value &&
                               std::is_destructible<T>::value &&
                               core::is_swappable<T>::value>
{
};
}


template <typename T>
struct is_semiregular : detail::is_semiregular_impl<T>
{
  using requirements = std::tuple<core::is_swappable<T>>;

  static constexpr int trigger_static_asserts()
  {
    static_assert(is_semiregular::value, "T is not Semiregular");
    return 1;
  }
};

template <typename T, typename = std::enable_if_t<is_semiregular<T>::value>>
using Semiregular = T;
}
};
}
}
}
