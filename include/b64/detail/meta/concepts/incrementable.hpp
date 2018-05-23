#pragma once

#include <type_traits>

#include <b64/detail/meta/aliases.hpp>
#include <b64/detail/meta/concepts/regular.hpp>
#include <b64/detail/meta/concepts/weakly_incrementable.hpp>
#include <b64/detail/meta/detected.hpp>

namespace b64
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
