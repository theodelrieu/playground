#pragma once

#include <type_traits>

#include <mgs/detail/meta/aliases.hpp>
#include <mgs/detail/meta/concepts/regular.hpp>
#include <mgs/detail/meta/concepts/weakly_incrementable.hpp>
#include <mgs/detail/meta/detected.hpp>

namespace mgs
{
inline namespace v1
{
namespace detail
{
template <typename T>
struct is_incrementable
  : std::integral_constant<
        bool,
        is_regular<T>::value &&
            is_detected_exact<T, post_increment_t, T&>::value &&
            is_weakly_incrementable<T>::value>
{
};
}
}
}
