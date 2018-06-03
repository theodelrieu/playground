#pragma once

#include <type_traits>

#include <mgs/detail/meta/aliases.hpp>
#include <mgs/detail/meta/concepts/semiregular.hpp>
#include <mgs/detail/meta/detected.hpp>

namespace mgs
{
namespace detail
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
