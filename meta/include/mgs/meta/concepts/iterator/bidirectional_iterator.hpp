#pragma once

#include <iterator>
#include <type_traits>

#include <mgs/meta/aliases/operators/post_decrement.hpp>
#include <mgs/meta/aliases/operators/pre_decrement.hpp>
#include <mgs/meta/aliases/types/iterator_category.hpp>
#include <mgs/meta/concepts/core/derived_from.hpp>
#include <mgs/meta/concepts/iterator/forward_iterator.hpp>
#include <mgs/meta/detected.hpp>

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace concepts
{
namespace iterator
{
template <typename T, typename = void>
struct is_bidirectional_iterator : std::false_type
{
};

template <typename T>
struct is_bidirectional_iterator<
    T,
    std::enable_if_t<is_forward_iterator<T>::value>>
{
  static constexpr auto const value =
      core::is_derived_from<detected_t<aliases::types::iterator_category,
                                       std::iterator_traits<T>>,
                            std::bidirectional_iterator_tag>::value &&
      is_detected_exact<T&, aliases::operators::pre_decrement, T&>::value &&
      is_detected_exact<T, aliases::operators::post_decrement, T&>::value;
};
}
}
namespace iterator_concepts = concepts::iterator;
}
}
}
