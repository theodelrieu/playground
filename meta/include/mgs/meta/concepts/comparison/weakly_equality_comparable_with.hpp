#pragma once

#include <tuple>
#include <type_traits>
#include <utility>

#include <mgs/meta/concepts/core/complete_type.hpp>

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
namespace detail
{
template <typename T, typename U, typename = void>
struct is_weakly_equality_comparable_with_impl : std::false_type
{
};

template <typename T, typename U>
struct is_weakly_equality_comparable_with_impl<
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

template <typename T, typename U>
struct is_weakly_equality_comparable_with : detail::is_weakly_equality_comparable_with_impl<T, U>
{
  using requirements = std::tuple<>;

  struct static_assert_t
  {
    static constexpr int trigger()
    {
      static_assert(is_weakly_equality_comparable_with::value,
                    "T is not WeaklyEqualityComparable with U");
      return 1;
    }
  };
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
