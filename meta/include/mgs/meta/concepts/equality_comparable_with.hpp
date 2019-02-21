#pragma once

#include <tuple>
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
namespace concepts
{
template <typename T, typename U>
struct is_equality_comparable_with
{
  using requirements = std::tuple<is_equality_comparable<T>,
                                  is_equality_comparable<U>,
                                  is_weakly_equality_comparable_with<T, U>>;

  static constexpr auto const value =
      is_equality_comparable<T>::value && is_equality_comparable<U>::value &&
      is_weakly_equality_comparable_with<T, U>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(is_equality_comparable_with::value,
                  "T is not EqualityComparableWith U");
    return 1;
  }
};

template <typename T, typename U>
constexpr auto is_equality_comparable_with_v =
    is_equality_comparable_with<T, U>::value;
}

template <typename T,
          typename U,
          typename = std::enable_if_t<
              concepts::is_equality_comparable_with<T, U>::value>>
using EqualityComparableWith = T;
}
}
}
