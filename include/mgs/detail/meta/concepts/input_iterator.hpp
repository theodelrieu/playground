#pragma once

#include <iterator>
#include <type_traits>

#include <mgs/detail/meta/aliases.hpp>
#include <mgs/detail/meta/concepts/derived_from.hpp>
#include <mgs/detail/meta/concepts/iterator.hpp>
#include <mgs/detail/meta/detected.hpp>

namespace mgs
{
namespace detail
{
template <typename T>
struct is_input_iterator
    : std::integral_constant<
          bool,
          is_iterator<T>::value &&
              is_derived_from<
                  detected_t<iterator_category_t, std::iterator_traits<T>>,
                  std::input_iterator_tag>::value>
{
};
}
}
