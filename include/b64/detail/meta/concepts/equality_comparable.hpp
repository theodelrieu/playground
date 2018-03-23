#pragma once

#include <type_traits>

// concept EqualityComparable:
// http://en.cppreference.com/w/cpp/concept/EqualityComparable
namespace b64
{
namespace detail
{
template <typename T, typename = void>
struct is_equality_comparable : std::false_type
{
};

template <typename T>
struct is_equality_comparable<
    T,
    std::enable_if_t<
        std::is_convertible<decltype(std::declval<T>() == std::declval<T>()),
                            bool>::value>> : std::true_type
{
};
}
}
