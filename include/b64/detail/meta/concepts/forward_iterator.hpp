#pragma once

#include <iterator>
#include <type_traits>

#include <b64/detail/meta/aliases.hpp>
#include <b64/detail/meta/concepts/derived_from.hpp>
#include <b64/detail/meta/concepts/incrementable.hpp>
#include <b64/detail/meta/concepts/input_iterator.hpp>
#include <b64/detail/meta/concepts/sentinel.hpp>
#include <b64/detail/meta/detected.hpp>

namespace b64
{
namespace detail
{
template <typename T>
struct is_forward_iterator
  : std::integral_constant<
        bool,
        is_input_iterator<T>::value &&
            is_derived_from<detected_t<iterator_category_t, T>,
                            std::forward_iterator_tag>::value &&
            is_sentinel<T, T>::value && is_incrementable<T>::value>
{
};
}
}

