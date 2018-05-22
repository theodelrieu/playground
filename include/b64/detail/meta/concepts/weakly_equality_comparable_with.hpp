#pragma once

#include <type_traits>

// http://en.cppreference.com/w/cpp/experimental/ranges/concepts/WeaklyEqualityComparableWith
namespace b64
{
namespace detail
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
}
}
