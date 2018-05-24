#pragma once

#include <iterator>
#include <type_traits>

#include <b64/detail/meta/aliases.hpp>
#include <b64/detail/meta/concepts/bidirectional_iterator.hpp>
#include <b64/detail/meta/concepts/derived_from.hpp>
#include <b64/detail/meta/concepts/sized_sentinel.hpp>
#include <b64/detail/meta/concepts/strict_totally_ordered.hpp>
#include <b64/detail/meta/detected.hpp>

namespace b64
{
namespace detail
{
template <typename T, typename = void>
struct is_random_access_iterator : std::false_type
{
};

template <typename T>
struct is_random_access_iterator<
    T,
    std::enable_if_t<is_bidirectional_iterator<T>::value &&
                     is_derived_from<detected_t<iterator_category_t,
                                                std::iterator_traits<T>>,
                                     std::random_access_iterator_tag>::value>>
{
private:
  using difference_type = difference_type_t<std::iterator_traits<T>>;

public:
  static auto constexpr value =
      is_strict_totally_ordered<T>::value && is_sized_sentinel<T, T>::value &&
      is_detected_exact<T&, addition_assignment_t, T&, difference_type>::
          value &&
      is_detected_exact<T, addition_t, T const, difference_type>::value &&
      is_detected_exact<T, addition_t, difference_type, T const>::value &&
      is_detected_exact<T, substraction_t, T const, difference_type>::value &&
      is_detected_exact<T&, substraction_assignment_t, T&, difference_type>::
          value &&
      std::is_convertible<
          detected_t<array_subscript_t, T const, difference_type>,
          reference_t<std::iterator_traits<T>>>::value &&
      std::is_convertible<detected_t<array_subscript_t, T, difference_type>,
                          reference_t<std::iterator_traits<T>>>::value;
};
}
}
