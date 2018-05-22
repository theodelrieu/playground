#pragma once

#include <type_traits>

#include <b64/detail/meta/concepts/equality_comparable.hpp>
#include <b64/detail/meta/concepts/weakly_equality_comparable_with.hpp>

// http://en.cppreference.com/w/cpp/experimental/ranges/concepts/EqualityComparable
namespace b64
{
namespace detail
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
