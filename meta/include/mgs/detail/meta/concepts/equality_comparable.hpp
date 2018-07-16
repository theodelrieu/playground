#pragma once

#include <type_traits>

#include <mgs/detail/meta/concepts/weakly_equality_comparable_with.hpp>

// http://en.cppreference.com/w/cpp/concept/EqualityComparable
namespace mgs
{
inline namespace v1
{
namespace detail
{
template <typename T>
struct is_equality_comparable : is_weakly_equality_comparable_with<T, T>
{
};
}
}
}
