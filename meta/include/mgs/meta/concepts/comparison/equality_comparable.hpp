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
  using requirements = std::tuple<>;

  struct static_assert_t
  {
    static constexpr int trigger()
    {
      static_assert(is_equality_comparable::value, "T is not EqualityComparable");
      return 1;
    }
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
