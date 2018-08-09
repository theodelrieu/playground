#pragma once

#include <type_traits>

#include <mgs/meta/aliases/operators/greater_or_equal_than.hpp>
#include <mgs/meta/aliases/operators/greater_than.hpp>
#include <mgs/meta/aliases/operators/less_or_equal_than.hpp>
#include <mgs/meta/aliases/operators/less_than.hpp>
#include <mgs/meta/concepts/comparison/strict_totally_ordered.hpp>
#include <mgs/meta/detected.hpp>

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace concepts
{
namespace comparison
{
// http://en.cppreference.com/w/cpp/experimental/ranges/concepts/StrictTotallyOrdered
template <typename T, typename U>
struct is_strict_totally_ordered_with
  : std::integral_constant<
        bool,
        is_strict_totally_ordered<T>::value &&
            is_strict_totally_ordered<U>::value &&
            is_detected_exact<bool,
                              aliases::operators::less_than,
                              T const&,
                              U const&>::value &&
            is_detected_exact<bool,
                              aliases::operators::less_or_equal_than,
                              T const&,
                              U const&>::value &&
            is_detected_exact<bool,
                              aliases::operators::greater_than,
                              T const&,
                              U const&>::value &&
            is_detected_exact<bool,
                              aliases::operators::greater_or_equal_than,
                              T const&,
                              U const&>::value &&
            is_detected_exact<bool,
                              aliases::operators::less_than,
                              U const&,
                              T const&>::value &&
            is_detected_exact<bool,
                              aliases::operators::less_or_equal_than,
                              U const&,
                              T const&>::value &&
            is_detected_exact<bool,
                              aliases::operators::greater_than,
                              U const&,
                              T const&>::value &&
            is_detected_exact<bool,
                              aliases::operators::greater_or_equal_than,
                              U const&,
                              T const&>::value>
{
};
}
}
namespace comparison_concepts = concepts::comparison;
}
}
}
