#pragma once

#include <type_traits>

#include <b64/detail/meta/aliases.hpp>
#include <b64/detail/meta/concepts/iterable.hpp>
#include <b64/detail/meta/concepts/iterator.hpp>
#include <b64/detail/meta/concepts/regular.hpp>
#include <b64/detail/meta/concepts/sentinel.hpp>
#include <b64/detail/meta/detected.hpp>

// template <Iterator, Sentinel>
// concept Encoder = requires(T const& v) {
//  requires Regular<T>;
//  requires Iterable<T>;
//  requires Constructible<T, Iterator, Sentinel>;
//  typename T::value_type;
//  typename T::difference_type;
//  requires(typename T::difference_type n) {
//    v.get();
//    v.seek_forward(n);
//    requires { v.get() } -> value_type const&;
//    requires { v.seek_forward(n) } -> void;
//  }
// }
namespace b64
{
namespace detail
{
template <typename T,
          typename = void>
struct is_encoder : std::false_type
{
};

template <typename T>
struct S;

template <typename T>
struct is_encoder<
    T,
    std::enable_if_t<is_iterable<T>::value && is_regular<T>::value &&
                     is_detected<value_type_t, T>::value &&
                     is_detected<difference_type_t, T>::value>>
{
private:
  using Iterator = detail2::result_of_begin_t<T>;
  using Sentinel = detail2::result_of_end_t<T>;
  S<Iterator> s;

  using value_type = typename T::value_type;
  using difference_type = typename T::difference_type;

public:
  static auto constexpr value =
      std::is_constructible<T, Iterator, Sentinel>::value &&
      is_detected_exact<value_type const&, get_function_t, T const&>::value &&
      is_detected_exact<void, seek_forward_function_t, T&, difference_type>::
          value;
};
}
}
