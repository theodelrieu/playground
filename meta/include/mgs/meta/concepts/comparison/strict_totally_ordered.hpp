#pragma once

#include <type_traits>

#include <mgs/meta/concepts/comparison/equality_comparable.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/greater_or_equal_than.hpp>
#include <mgs/meta/detected/operators/greater_than.hpp>
#include <mgs/meta/detected/operators/less_or_equal_than.hpp>
#include <mgs/meta/detected/operators/less_than.hpp>

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
template <typename T>
struct is_strict_totally_ordered
  : std::integral_constant<
        bool,
        is_equality_comparable<T>::value &&
            is_detected_exact<bool,
                              detected::operators::less_than,
                              T const&,
                              T const&>::value &&
            is_detected_exact<bool,
                              detected::operators::less_or_equal_than,
                              T const&,
                              T const&>::value &&
            is_detected_exact<bool,
                              detected::operators::greater_than,
                              T const&,
                              T const&>::value &&
            is_detected_exact<bool,
                              detected::operators::greater_or_equal_than,
                              T const&,
                              T const&>::value>
{
};
}
}
namespace comparison_concepts = concepts::comparison;
}
}
}
