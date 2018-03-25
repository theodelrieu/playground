#pragma once

#include <cstdint>
#include <type_traits>

#include <b64/detail/meta/concepts/equality_comparable.hpp>
#include <b64/detail/meta/concepts/swappable.hpp>
#include <b64/detail/meta/detected.hpp>

// concept EncodingProcessor = requires(T const& processor) {
//   requires EqualityComparable<T>;
//   requires DefaultConstructible<T>;
//   requires CopyConstructible<T>;
//   requires CopyAssignable<T>;
//   requires Destructible<T>;
//   requires Swappable<T>;
//   typename T::value_type;
//   { processor.next_char() } -> typename T::value_type;
//   { processor.eof() } -> bool;
// }

namespace b64
{
namespace detail
{
template <typename T, typename = void>
struct is_encoding_processor : std::false_type
{
};

template <typename T>
using next_char_t = decltype(std::declval<T const&>().next_char());

template <typename T>
using eof_t = decltype(std::declval<T const&>().eof());

template <typename T>
using value_type_t = typename T::value_type;

template <typename T>
struct has_next_char_method
    : std::integral_constant<bool,
                             std::is_convertible<detected_t<next_char_t, T>,
                                                 typename T::value_type>::value>
{
};

template <typename T>
struct has_eof_method
    : std::integral_constant<
          bool,
          std::is_convertible<detected_t<eof_t, T>, bool>::value>
{
};

template <typename T>
struct is_encoding_processor<
    T,
    std::enable_if_t<
        is_equality_comparable<T>::value && is_swappable<T>::value &&
        std::is_default_constructible<T>::value &&
        std::is_copy_constructible<T>::value &&
        std::is_copy_assignable<T>::value && std::is_destructible<T>::value &&
        std::is_integral<detail::detected_t<value_type_t, T>>::value &&
        !std::is_same<detail::detected_t<value_type_t, T>, bool>::value &&
        has_eof_method<T>::value && has_next_char_method<T>::value>>
    : std::true_type
{
};
}
}
