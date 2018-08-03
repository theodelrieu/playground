#pragma once

#include <type_traits>

#include <mgs/meta/concepts/equality_comparable.hpp>
#include <mgs/meta/concepts/semiregular.hpp>

namespace mgs
{
inline namespace v1
{
namespace meta
{
template <typename T, typename = void>
struct is_regular : std::integral_constant<bool,
                                           is_semiregular<T>::value &&
                                               is_equality_comparable<T>::value>
{
};
}
}
}
