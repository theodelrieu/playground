#pragma once

#include <iterator>
#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/core/derived_from.hpp>
#include <mgs/meta/concepts/iterator/iterator.hpp>
#include <mgs/meta/concepts/iterator/readable.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/iter_concept.hpp>
#include <mgs/meta/iterator_traits.hpp>

// https://en.cppreference.com/w/cpp/experimental/ranges/iterator/InputIterator

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
struct is_input_iterator
{
private:
  static constexpr auto const has_correct_category =
      core::is_derived_from<detected_t<meta::iter_concept, T>,
                            std::input_iterator_tag>::value;

public:
  using requirements = std::tuple<is_iterator<T>, is_readable<T>>;

  static constexpr auto const value =
      has_correct_category && is_iterator<T>::value && is_readable<T>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not an InputIterator");
    static_assert(
        has_correct_category,
        "Iterator category tag must derive from std::input_iterator_tag");
    return 1;
  }
};

template <typename T, typename = std::enable_if_t<is_input_iterator<T>::value>>
using InputIterator = T;
}
};
}
}
}
