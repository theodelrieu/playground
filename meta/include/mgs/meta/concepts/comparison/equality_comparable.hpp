#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/comparison/weakly_equality_comparable_with.hpp>

// http://en.cppreference.com/w/cpp/concept/EqualityComparable
namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace concepts
{
namespace comparison
{
template <typename T>
struct is_equality_comparable : is_weakly_equality_comparable_with<T, T>
{
  using requirements = std::tuple<is_weakly_equality_comparable_with<T, T>>;

  struct static_assert_t
  {
    static_assert(is_equality_comparable::value, "T is not EqualityComparable");
  };
};

template <typename T,
          typename = std::enable_if_t<is_equality_comparable<T>::value>>
using EqualityComparable = T;
}
};
}
}
}
