#pragma once

#include <type_traits>

#include <mgs/detail/meta/concepts/equality_comparable.hpp>
#include <mgs/detail/meta/concepts/semiregular.hpp>

namespace mgs
{
inline namespace v1
{
namespace detail
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
