#pragma once

#include <iterator>
#include <type_traits>

#include <mgs/meta/concepts/core/derived_from.hpp>
#include <mgs/meta/concepts/iterator/forward_iterator.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/post_decrement.hpp>
#include <mgs/meta/detected/operators/pre_decrement.hpp>
#include <mgs/meta/detected/types/iterator_category.hpp>

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
      core::is_derived_from<detected_t<detected::types::iterator_category,
                                       std::iterator_traits<T>>,
                            std::bidirectional_iterator_tag>::value &&
      is_detected_exact<T&, detected::operators::pre_decrement, T&>::value &&
      is_detected_exact<T, detected::operators::post_decrement, T&>::value;
};

template <typename T,
          typename = std::enable_if_t<is_bidirectional_iterator<T>::value>>
using BidirectionalIterator = T;
}
};
}
}
}
