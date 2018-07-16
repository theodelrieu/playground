#pragma once

#include <iterator>
#include <type_traits>

#include <mgs/detail/meta/aliases.hpp>
#include <mgs/detail/meta/concepts/derived_from.hpp>
#include <mgs/detail/meta/concepts/forward_iterator.hpp>
#include <mgs/detail/meta/detected.hpp>

namespace mgs
{
inline namespace v1
{
namespace detail
{
template <typename T>
struct is_bidirectional_iterator
    : std::integral_constant<
          bool,
          is_forward_iterator<T>::value &&
              is_derived_from<
                  detected_t<iterator_category_t, std::iterator_traits<T>>,
                  std::bidirectional_iterator_tag>::value &&
              is_detected_exact<T&, pre_decrement_t, T&>::value &&
              is_detected_exact<T, post_decrement_t, T&>::value>
{
};
}
}
}
