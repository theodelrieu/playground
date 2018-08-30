#pragma once

#include <type_traits>
#include <utility>

// http://en.cppreference.com/w/cpp/experimental/ranges/concepts/WeaklyEqualityComparableWith
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
template <typename T, typename U, typename = void>
struct is_weakly_equality_comparable_with : std::false_type
{
};

template <typename T, typename U>
struct is_weakly_equality_comparable_with<
    T,
    U,
    std::enable_if_t<std::is_convertible<decltype(std::declval<T const&>() ==
                                                  std::declval<U const&>()),
                                         bool>::value &&
                     std::is_convertible<decltype(std::declval<U const&>() ==
                                                  std::declval<T const&>()),
                                         bool>::value &&
                     std::is_convertible<decltype(std::declval<T const&>() !=
                                                  std::declval<U const&>()),
                                         bool>::value &&
                     std::is_convertible<decltype(std::declval<U const&>() !=
                                                  std::declval<T const&>()),
                                         bool>::value>> : std::true_type
{
};

template <typename T,
          typename U,
          typename =
              std::enable_if_t<is_weakly_equality_comparable_with<T, U>::value>>
using WeaklyEqualityComparableWith = T;
}
};
}
}
}
