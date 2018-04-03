#pragma once

#include <type_traits>

#include <b64/detail/meta/concepts/iterator.hpp>
#include <b64/detail/meta/concepts/equality_comparable_with.hpp>

// concept Sentinel = requires(T a, Iterator b) {
//  requires EqualityComparableWith<T, Iterator>;
//  requires DefaultConstructible<T>;
//  requires CopyConstructible<T>;
//  requires CopyAssignable<T>;
//  requires Destructible<T>;
//  requires Swappable<T>;
// }
namespace b64
{
namespace detail
{
template <typename T, typename Iterator>
struct is_sentinel : std::integral_constant<
                         bool,
                         is_iterator<Iterator>::value &&
                             is_equality_comparable_with<T, Iterator>::value &&
                             is_swappable<T>::value &&
                             std::is_default_constructible<T>::value &&
                             std::is_copy_constructible<T>::value &&
                             std::is_copy_assignable<T>::value &&
                             std::is_destructible<T>::value>
{
};
}
}
