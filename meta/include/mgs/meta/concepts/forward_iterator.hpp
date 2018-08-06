#pragma once

#include <iterator>
#include <type_traits>

#include <mgs/meta/aliases.hpp>
#include <mgs/meta/concepts/derived_from.hpp>
#include <mgs/meta/concepts/incrementable.hpp>
#include <mgs/meta/concepts/input_iterator.hpp>
#include <mgs/meta/concepts/sentinel.hpp>
#include <mgs/meta/detected.hpp>

namespace mgs
{
inline namespace v1
{
namespace meta
{
template <typename T>
struct is_forward_iterator
  : std::integral_constant<
        bool,
        is_input_iterator<T>::value &&
            is_derived_from<
                detected_t<iterator_category_t, std::iterator_traits<T>>,
                std::forward_iterator_tag>::value &&
            is_sentinel<T, T>::value && is_incrementable<T>::value>
{
};
}
}
}
