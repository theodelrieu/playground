#pragma once

#include <type_traits>

#include <b64/detail/meta/concepts/equality_comparable.hpp>
#include <b64/detail/meta/concepts/iterable.hpp>
#include <b64/detail/meta/concepts/iterator.hpp>
#include <b64/detail/meta/concepts/sentinel.hpp>

// template <Iterator, Sentinel>
// concept Encoder = requires(T const& v) {
//  requires Iterable<T>;
//  requires Constructible<T, Iterator, Sentinel>;
//  requires EqualityComparable<T>;
// }

namespace b64
{
namespace detail
{
template <typename T,
          typename Iterator,
          typename Sentinel = Iterator,
          typename = void>
struct is_encoder : std::false_type
{
};

template <typename T, typename Iterator, typename Sentinel>
struct is_encoder<
    T,
    Iterator,
    Sentinel,
    std::enable_if_t<is_iterable<T>::value &&
                     is_equality_comparable<T>::value &&
                     std::is_constructible<T, Iterator, Sentinel>::value>>
    : std::true_type
{
};
}
}
