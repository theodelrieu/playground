#pragma once

#include <iterator>
#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/core/derived_from.hpp>
#include <mgs/meta/concepts/iterator/incrementable.hpp>
#include <mgs/meta/concepts/iterator/input_iterator.hpp>
#include <mgs/meta/concepts/iterator/sentinel.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/types/iterator_category.hpp>

// https://en.cppreference.com/w/cpp/named_req/ForwardIterator

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
struct is_forward_iterator
{
  using requirements = std::tuple<is_input_iterator<T>, is_sentinel<T, T>, is_incrementable<T>>;

  static constexpr auto const has_correct_tag = core::is_derived_from<
      detected_t<detected::types::iterator_category, std::iterator_traits<T>>,
      std::forward_iterator_tag>::value;

  // since we don't have OutputIterator concept, do not check it reference is T& or T const&.
  static constexpr auto const has_correct_reference = std::is_reference<
      detected_t<detected::types::reference, std::iterator_traits<T>>>::value;

  static constexpr auto const value =
      is_input_iterator<T>::value && has_correct_tag && has_correct_reference &&
      is_sentinel<T, T>::value && is_incrementable<T>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not a ForwardIterator");

    static_assert(has_correct_reference,
                  "'std::iterator_traits<T>::reference' must be "
                  "'std::iterator_traits<T>::value_type' & or const&");
    static_assert(has_correct_tag,
                  "'std::iterator_traits<T>::iterator_category' is not derived "
                  "from 'std::forward_iterator_tag'");
    return 1;
  }
};

template <typename T,
          typename = std::enable_if_t<is_forward_iterator<T>::value>>
using ForwardIterator = T;
}
};
}
}
}
