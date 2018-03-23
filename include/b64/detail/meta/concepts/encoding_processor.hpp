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
//   { processor.next_char() } -> std::uint8_t;
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
using next_char_t = decltype(std::declval<std::add_const_t<T>>().next_char());

template <typename T>
using eof_t = decltype(std::declval<std::add_const_t<T>>().eof());

template <typename T>
struct has_next_char_method
  : std::integral_constant<
        bool,
        std::is_same<std::uint8_t, detected_t<next_char_t, T>>::value>
{
};

template <typename T>
struct has_eof_method
  : std::integral_constant<bool,
                           std::is_same<bool, detected_t<eof_t, T>>::value>
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
        has_eof_method<T>::value && has_next_char_method<T>::value>>
  : std::true_type
{
};
}
}
