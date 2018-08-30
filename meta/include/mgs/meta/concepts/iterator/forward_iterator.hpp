#pragma once

#include <iterator>
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
template <typename T, typename = void>
struct is_forward_iterator : std::false_type
{
};

template <typename T>
struct is_forward_iterator<T, std::enable_if_t<is_input_iterator<T>::value>>
{
  static constexpr auto const value =
      core::is_derived_from<detected_t<detected::types::iterator_category,
                                       std::iterator_traits<T>>,
                            std::forward_iterator_tag>::value &&
      is_sentinel<T, T>::value && is_incrementable<T>::value;
};

template <typename T,
          typename = std::enable_if_t<is_forward_iterator<T>::value>>
using ForwardIterator = T;
}
};
}
}
}
