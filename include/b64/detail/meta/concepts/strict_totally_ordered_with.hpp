#pragma once

#include <type_traits>

#include <b64/detail/meta/aliases.hpp>
#include <b64/detail/meta/concepts/strict_totally_ordered.hpp>
#include <b64/detail/meta/detected.hpp>

namespace b64
{
namespace detail
{
// http://en.cppreference.com/w/cpp/experimental/ranges/concepts/StrictTotallyOrdered
template <typename T, typename U>
struct is_strict_totally_ordered_with
  : std::integral_constant<
        bool,
        is_strict_totally_ordered<T>::value &&
            is_strict_totally_ordered<U>::value &&
            is_detected_exact<bool, less_than_t, T const&, U const&>::value &&
            is_detected_exact<bool, less_or_equal_than_t, T const&, U const&>::
                value &&
            is_detected_exact<bool, greater_than_t, T const&, U const&>::
                value &&
            is_detected_exact<bool,
                              greater_or_equal_than_t,
                              T const&,
                              U const&>::value &&
            is_detected_exact<bool, less_than_t, U const&, T const&>::value &&
            is_detected_exact<bool, less_or_equal_than_t, U const&, T const&>::
                value &&
            is_detected_exact<bool, greater_than_t, U const&, T const&>::
                value &&
            is_detected_exact<bool,
                              greater_or_equal_than_t,
                              U const&,
                              T const&>::value>
{
};
}
}
