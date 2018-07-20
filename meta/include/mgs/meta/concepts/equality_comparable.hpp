#pragma once

#include <type_traits>

#include <mgs/meta/concepts/weakly_equality_comparable_with.hpp>

// http://en.cppreference.com/w/cpp/concept/EqualityComparable
namespace mgs
{
namespace meta
{
inline namespace v1
{
template <typename T>
struct is_equality_comparable : is_weakly_equality_comparable_with<T, T>
{
};
}
}
}
