#pragma once

#include <iterator>
#include <type_traits>

#include <b64/detail/meta/aliases.hpp>
#include <b64/detail/meta/concepts/derived_from.hpp>
#include <b64/detail/meta/concepts/forward_iterator.hpp>
#include <b64/detail/meta/detected.hpp>

namespace b64
{
namespace detail
{
template <typename T>
struct is_bidirectional_iterator
  : std::integral_constant<
        bool,
        is_forward_iterator<T>::value &&
            is_derived_from<detected_t<iterator_category_t, T>,
                            std::bidirectional_iterator_tag>::value &&
            is_detected_exact<T&, pre_decrement_t, T>::value &&
            is_detected_exact<T, post_decrement_t, T>::value>
{
};
}
}

