#pragma once

#include <iterator>
#include <type_traits>

#include <b64/detail/meta/aliases.hpp>
#include <b64/detail/meta/concepts/swappable.hpp>
#include <b64/detail/meta/detected.hpp>

// concept Iterator: http://en.cppreference.com/w/cpp/concept/Iterator
namespace b64
{
namespace detail
{
template <typename T>
struct is_valid_iterator_traits : std::false_type
{
};

template <typename T>
struct is_valid_iterator_traits<std::iterator_traits<T>>
{
private:
  using traits = std::iterator_traits<T>;

public:
  static constexpr auto value =
      is_detected<value_type_t, traits>::value &&
      is_detected<difference_type_t, traits>::value &&
      is_detected<pointer_t, traits>::value &&
      is_detected<iterator_category_t, traits>::value &&
      is_detected<reference_t, traits>::value;
};

template <typename T>
using dereference_t = decltype(*std::declval<T&>());

template <typename T>
using increment_t = decltype(++std::declval<T&>());

template <typename T>
struct is_iterator
  : std::integral_constant<
        bool,
        std::is_copy_constructible<T>::value &&
            std::is_copy_assignable<T>::value &&
            std::is_destructible<T>::value && is_swappable<T>::value &&
            is_valid_iterator_traits<std::iterator_traits<T>>::value &&
            is_detected<dereference_t, T>::value &&
            is_detected_exact<T&, increment_t, T>::value>
{
};
}
}
