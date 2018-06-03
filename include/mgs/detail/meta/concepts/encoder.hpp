#pragma once

#include <type_traits>

#include <mgs/detail/meta/aliases.hpp>
#include <mgs/detail/meta/concepts/iterable.hpp>
#include <mgs/detail/meta/concepts/iterator.hpp>
#include <mgs/detail/meta/concepts/regular.hpp>
#include <mgs/detail/meta/concepts/sentinel.hpp>
#include <mgs/detail/meta/detected.hpp>

// template <Iterator, Sentinel>
// concept Encoder = requires(T const& v, T& u) {
//  requires Regular<T>;
//  requires Iterable<T>;
//  requires Constructible<T, Iterator, Sentinel>;
//  typename T::value_type;
//  typename T::difference_type;
//  typename T::underlying_iterator;
//  typename T::underlying_sentinel;
//  requires(typename T::difference_type n) {
//    v.get();
//    u.seek_forward(n);
//    requires { v.get() } -> value_type const&;
//    requires { u.seek_forward(n) } -> void;
//  }
// }
namespace mgs
{
namespace detail
{
template <typename T,
          typename = void>
struct is_encoder : std::false_type
{
};

template <typename T>
struct is_encoder<
    T,
    std::enable_if_t<is_iterable<T>::value && is_regular<T>::value &&
                     is_detected<value_type_t, T>::value &&
                     is_detected<underlying_iterator_t, T>::value &&
                     is_detected<underlying_sentinel_t, T>::value &&
                     is_detected<difference_type_t, T>::value>>
{
private:
  using value_type = typename T::value_type;
  using difference_type = typename T::difference_type;

public:
  static auto constexpr value =
      std::is_constructible<T,
                            underlying_iterator_t<T>,
                            underlying_sentinel_t<T>>::value &&
      is_detected_exact<value_type const&, get_function_t, T const&>::value &&
      is_detected_exact<void, seek_forward_function_t, T&, difference_type>::
          value;
};
}
}
