#pragma once

#include <iterator>
#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/core/derived_from.hpp>
#include <mgs/meta/concepts/iterator/iterator.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/types/iterator_category.hpp>

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
namespace detail
{
template <typename T, typename = void>
struct is_input_iterator_impl : std::false_type
{
};

template <typename T>
struct is_input_iterator_impl<T, std::enable_if_t<is_iterator<T>::value>>
{
  using traits = std::iterator_traits<T>;
  using value_type = detected_t<detected::types::value_type, traits>;

  static constexpr auto const value =
      core::is_derived_from<
          detected_t<detected::types::iterator_category, traits>,
          std::input_iterator_tag>::value &&
      is_detected_convertible<value_type,
                              detected::operators::dereference,
                              T&>::value &&
      is_detected_exact<typename traits::reference,
                        detected::operators::dereference,
                        T&>::value &&
      is_detected_convertible<value_type,
                              detected::operators::dereference,
                              detected::operators::post_increment<T&>>::value;
};
}

template <typename T>
struct is_input_iterator : detail::is_input_iterator_impl<T>
{
  using requirements = std::tuple<is_iterator<T>>;

  using traits = std::iterator_traits<T>;
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

  static constexpr int trigger_static_asserts()
  {
    static_assert(is_input_iterator::value, "T is not an InputIterator");
    static_assert(has_correct_tag,
                  "'std::iterator_traits<T>::iterator_category' is not derived "
                  "from 'std::input_iterator_tag'");
    static_assert(has_dereference_value_type,
                  "Invalid or missing 'T operator*': must return a type "
                  "convertible to 'std::iterator_traits<T>::value_type'");
    static_assert(has_dereference_reference,
                  "Invalid or missing 'T operator*': must return "
                  "'std::iterator_traits<T>::reference'");
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
