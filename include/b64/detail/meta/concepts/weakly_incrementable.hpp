#pragma once

#include <type_traits>

#include <b64/detail/meta/aliases.hpp>
#include <b64/detail/meta/concepts/semiregular.hpp>
#include <b64/detail/meta/detected.hpp>

namespace b64
{
namespace detail
{
template <typename T>
struct is_weakly_incrementable
  : std::integral_constant<
        bool,
        is_semiregular<T>::value &&
            is_detected_exact<T&, pre_increment_t, T>::value &&
            is_detected<post_increment_t, T>::value>
{
};
}
}
