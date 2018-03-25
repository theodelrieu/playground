#pragma once

#include <type_traits>

#include <b64/detail/meta/concepts/equality_comparable_with.hpp>

// FIXME point to Ranges TS
// concept EqualityComparable:
// http://en.cppreference.com/w/cpp/concept/EqualityComparable
namespace b64
{
namespace detail
{
template <typename T>
struct is_equality_comparable : is_equality_comparable_with<T, T>
{
};
}
}
