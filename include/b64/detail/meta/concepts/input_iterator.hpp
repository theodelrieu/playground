#pragma once

#include <iterator>
#include <type_traits>

#include <b64/detail/meta/aliases.hpp>
#include <b64/detail/meta/concepts/derived_from.hpp>
#include <b64/detail/meta/concepts/iterator.hpp>
#include <b64/detail/meta/detected.hpp>

namespace b64
{
namespace detail
{
template <typename T>
struct is_input_iterator
  : std::integral_constant<
        bool,
        is_iterator<T>::value &&
            is_derived_from<detected_t<iterator_category_t, T>,
                            std::input_iterator_tag>::value>
{
};
}
}
