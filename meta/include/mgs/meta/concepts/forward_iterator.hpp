#pragma once

#include <iterator>
#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/derived_from.hpp>
#include <mgs/meta/concepts/incrementable.hpp>
#include <mgs/meta/concepts/input_iterator.hpp>
#include <mgs/meta/concepts/sentinel.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/types/iterator_category.hpp>
#include <mgs/meta/iter_concept.hpp>

// https://en.cppreference.com/w/cpp/experimental/ranges/ForwardIterator

namespace mgs
{
namespace meta
{
template <typename T>
struct is_forward_iterator
{
private:
  static constexpr auto const has_correct_category =
      is_derived_from<detected_t<meta::iter_concept, T>,
                      std::forward_iterator_tag>::value;

public:
  using requirements =
      std::tuple<is_input_iterator<T>, is_incrementable<T>, is_sentinel<T, T>>;

  static constexpr auto const value =
      is_input_iterator<T>::value && has_correct_category &&
      is_sentinel<T, T>::value && is_incrementable<T>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not a ForwardIterator");

    static_assert(
        has_correct_category,
        "Iterator category tag must derive from std::forward_iterator_tag");
    return 1;
  }
};

template <typename T>
constexpr auto is_forward_iterator_v = is_forward_iterator<T>::value;

template <typename T,
          typename = std::enable_if_t<is_forward_iterator<T>::value>>
using ForwardIterator = T;
}
}
