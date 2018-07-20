#pragma once

#include <type_traits>

#include <mgs/meta/aliases.hpp>
#include <mgs/meta/concepts/regular.hpp>
#include <mgs/meta/concepts/weakly_incrementable.hpp>
#include <mgs/meta/detected.hpp>

namespace mgs
{
namespace meta
{
inline namespace v1
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
