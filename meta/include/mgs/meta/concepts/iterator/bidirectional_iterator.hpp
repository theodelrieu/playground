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
template <typename T>
struct is_bidirectional_iterator
{
  using requirements = std::tuple<is_forward_iterator<T>>;
  using lvalue_ref = std::add_lvalue_reference_t<T>;

  static constexpr auto const has_correct_tag = core::is_derived_from<
      detected_t<detected::types::iterator_category, std::iterator_traits<T>>,
      std::bidirectional_iterator_tag>::value;

  static constexpr auto const has_pre_decrement =
      is_detected_exact<lvalue_ref,
                        detected::operators::pre_decrement,
                        lvalue_ref>::value;

  static constexpr auto const has_post_decrement = 
      is_detected_exact<T, detected::operators::post_decrement, lvalue_ref>::
          value;

  static constexpr auto const value = is_forward_iterator<T>::value &&
                                      has_correct_tag && has_pre_decrement &&
                                      has_post_decrement;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not a BidirectionalIterator");
    static_assert(has_correct_tag,
                  "'std::iterator_traits<T>::iterator_category' is not derived "
                  "from 'std::bidirectional_iterator_tag'");
    static_assert(has_pre_decrement,
                  "Missing or invalid operator: 'T& operator--()'");
    static_assert(has_post_decrement,
                  "Missing or invalid operator: 'T operator--(int)'");
    return 1;
  }
};

template <typename T,
          typename = std::enable_if_t<is_bidirectional_iterator<T>::value>>
using BidirectionalIterator = T;
}
};
}
}
}
