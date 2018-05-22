#pragma once

#include <type_traits>

#include <b64/detail/meta/concepts/equality_comparable.hpp>
#include <b64/detail/meta/concepts/semiregular.hpp>

namespace b64
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
