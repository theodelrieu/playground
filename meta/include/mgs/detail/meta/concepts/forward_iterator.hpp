#pragma once

#include <iterator>
#include <type_traits>

#include <mgs/detail/meta/aliases.hpp>
#include <mgs/detail/meta/concepts/derived_from.hpp>
#include <mgs/detail/meta/concepts/incrementable.hpp>
#include <mgs/detail/meta/concepts/input_iterator.hpp>
#include <mgs/detail/meta/concepts/sentinel.hpp>
#include <mgs/detail/meta/detected.hpp>

namespace mgs
{
namespace detail
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
