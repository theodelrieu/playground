#pragma once

#include <type_traits>

#include <mgs/meta/concepts/comparison/equality_comparable.hpp>
#include <mgs/meta/concepts/object/semiregular.hpp>

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
struct is_regular
  : std::integral_constant<bool,
                           is_semiregular<T>::value &&
                               comparison::is_equality_comparable<T>::value>
{
};
}
}
namespace object_concepts = concepts::object;
}
}
}
