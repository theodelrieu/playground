#pragma once

#include <type_traits>

#include <mgs/meta/aliases.hpp>
#include <mgs/meta/concepts/equality_comparable.hpp>
#include <mgs/meta/detected.hpp>

namespace mgs
{
namespace meta
{
inline namespace v1
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
}
