#pragma once

#include <type_traits>

// concept EqualityComparableWith:
// TODO link Range TS cppreference, try to understand what the hell is ranges::CommonReference
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
}
}
