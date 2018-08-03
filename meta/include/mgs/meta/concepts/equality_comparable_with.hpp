#pragma once

#include <type_traits>

#include <mgs/meta/concepts/equality_comparable.hpp>
#include <mgs/meta/concepts/weakly_equality_comparable_with.hpp>

// http://en.cppreference.com/w/cpp/experimental/ranges/concepts/EqualityComparable
namespace mgs
{
inline namespace v1
{
namespace meta
{
template <typename T, typename U, typename = void>
struct is_equality_comparable_with : std::false_type
{
};

template <typename T, typename U>
struct is_equality_comparable_with<
    T,
    U,
    std::enable_if_t<is_equality_comparable<T>::value &&
                     is_equality_comparable<U>::value &&
                     is_weakly_equality_comparable_with<T, U>::value>>
  : std::true_type
{
};
}
}
}
