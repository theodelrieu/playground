#pragma once

#include <iterator>
#include <type_traits>

#include <mgs/meta/concepts/core/derived_from.hpp>
#include <mgs/meta/concepts/iterator/iterator.hpp>
#include <mgs/meta/detected.hpp>
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
template <typename T, typename = void>
struct is_input_iterator : std::false_type
{
};

template <typename T>
struct is_input_iterator<T, std::enable_if_t<is_iterator<T>::value>>
{
  static constexpr auto const value = core::is_derived_from<
      detected_t<detected::types::iterator_category, std::iterator_traits<T>>,
      std::input_iterator_tag>::value;
};
}
}
namespace iterator_concepts = concepts::iterator;
}
}
}
