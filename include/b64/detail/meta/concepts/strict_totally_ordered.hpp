#pragma once

#include <type_traits>

#include <b64/detail/meta/aliases.hpp>
#include <b64/detail/meta/concepts/equality_comparable.hpp>
#include <b64/detail/meta/detected.hpp>

namespace b64
{
namespace detail
{
template <typename T>
struct is_strict_totally_ordered
  : std::integral_constant<
        bool,
        is_equality_comparable<T>::value &&
            is_detected_exact<bool, less_than_t, T const&, T const&>::value &&
            is_detected_exact<bool, less_or_equal_than_t, T const&, T const&>::
                value &&
            is_detected_exact<bool, greater_than_t, T const&, T const&>::
                value &&
            is_detected_exact<bool,
                              greater_or_equal_than_t,
                              T const&,
                              T const&>::value>
{
};
}
}
