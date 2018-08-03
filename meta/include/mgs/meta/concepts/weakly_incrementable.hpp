#pragma once

#include <type_traits>

#include <mgs/meta/aliases.hpp>
#include <mgs/meta/concepts/semiregular.hpp>
#include <mgs/meta/detected.hpp>

namespace mgs
{
inline namespace v1
{
namespace meta
{
template <typename T>
struct is_weakly_incrementable
  : std::integral_constant<
        bool,
        is_semiregular<T>::value &&
            is_detected_exact<T&, pre_increment_t, T&>::value &&
            is_detected<post_increment_t, T&>::value>
{
};
}
}
}
