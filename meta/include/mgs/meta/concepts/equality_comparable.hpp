#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/weakly_equality_comparable_with.hpp>

// http://en.cppreference.com/w/cpp/concept/EqualityComparable
namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace concepts
{
template <typename T>
struct is_equality_comparable : is_weakly_equality_comparable_with<T, T>
{
  using requirements = std::tuple<>;

  static constexpr int trigger_static_asserts()
  {
    static_assert(is_equality_comparable::value, "T is not EqualityComparable");
    return 1;
  }
};
}

template <typename T,
          typename =
              std::enable_if_t<concepts::is_equality_comparable<T>::value>>
using EqualityComparable = T;
}
}
}
