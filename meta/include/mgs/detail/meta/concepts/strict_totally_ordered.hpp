#pragma once

#include <type_traits>

#include <mgs/detail/meta/aliases.hpp>
#include <mgs/detail/meta/concepts/equality_comparable.hpp>
#include <mgs/detail/meta/detected.hpp>

namespace mgs
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
