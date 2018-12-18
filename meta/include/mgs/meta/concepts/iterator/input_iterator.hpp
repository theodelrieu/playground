#pragma once

#include <iterator>
#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/core/derived_from.hpp>
#include <mgs/meta/concepts/iterator/iterator.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/types/iterator_category.hpp>
#include <mgs/meta/iterator_traits.hpp>

// https://en.cppreference.com/w/cpp/experimental/ranges/iterator/InputIterator
// Note: inspired from the above link, without CommonReference/Readable etc...

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
  using traits = meta::iterator_traits<T>;

  using value_type = detected_t<detected::types::value_type, traits>;
  using lvalue_ref = std::add_lvalue_reference_t<T>;

  static constexpr auto const has_correct_tag = core::is_derived_from<
      detected_t<detected::types::iterator_category, traits>,
      std::input_iterator_tag>::value;

  static constexpr auto const has_dereference_value_type =
      is_detected_convertible<value_type,
                              detected::operators::dereference,
                              lvalue_ref>::value;

  static constexpr auto const has_dereference_reference =
      is_detected_exact<detected_t<detected::types::reference, traits>,
                        detected::operators::dereference,
                        lvalue_ref>::value;

  static constexpr auto const has_post_increment_dereference =
      is_detected_convertible<
          value_type,
          detected::operators::dereference,
          detected_t<detected::operators::post_increment, lvalue_ref>>::value;

public:
  using requirements = std::tuple<is_iterator<T>>;

  static constexpr auto const value =
      has_correct_tag && has_dereference_reference &&
      has_dereference_value_type && has_post_increment_dereference;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not an InputIterator");
    static_assert(has_correct_tag,
                  "'std::iterator_traits<T>::iterator_category' is not derived "
                  "from 'std::input_iterator_tag'");
    static_assert(has_dereference_value_type,
                  "Invalid or missing 'T operator*': must return a type "
                  "convertible to 'std::iterator_traits<T>::value_type'");
    static_assert(has_dereference_reference,
                  "Invalid or missing 'T operator*': must return "
                  "'std::iterator_traits<T>::reference'");
    static_assert(has_post_increment_dereference,
                  "Invalid expression: 'T t; *t++;': Must return a type "
                  "convertible to 'std::iterator_traits<T>::value_type'");
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
