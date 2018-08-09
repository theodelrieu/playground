#pragma once

#include <type_traits>

#include <mgs/meta/concepts/core/swappable.hpp>

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
template <typename T, typename = void>
struct is_semiregular
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
}
namespace object_concepts = concepts::object;
}
}
}
