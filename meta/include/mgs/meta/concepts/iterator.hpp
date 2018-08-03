#pragma once

#include <iterator>
#include <type_traits>

#include <mgs/meta/aliases.hpp>
#include <mgs/meta/concepts/semiregular.hpp>
#include <mgs/meta/detected.hpp>

// concept Iterator: http://en.cppreference.com/w/cpp/concept/Iterator
namespace mgs
{
inline namespace v1
{
namespace meta
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
struct is_iterator
    : std::integral_constant<
          bool,
          is_semiregular<T>::value &&
              is_valid_iterator_traits<std::iterator_traits<T>>::value &&
              is_detected<dereference_t, T&>::value &&
              is_detected_exact<T&, pre_increment_t, T&>::value>
{
};
}
}
}
