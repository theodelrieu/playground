#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/weakly_equality_comparable_with.hpp>
#include <mgs/meta/concepts/iterator.hpp>
#include <mgs/meta/concepts/semiregular.hpp>

// http://en.cppreference.com/w/cpp/experimental/ranges/Sentinel

namespace mgs
{
namespace meta
{
namespace concepts
{
template <typename T, typename Iterator>
struct is_sentinel
  : std::integral_constant<
        bool,
        is_semiregular<T>::value && is_iterator<Iterator>::value &&
            is_weakly_equality_comparable_with<T, Iterator>::value>
{
  using requirements =
      std::tuple<is_semiregular<T>,
                 is_iterator<Iterator>,
                 is_weakly_equality_comparable_with<T, Iterator>>;

  static constexpr int trigger_static_asserts()
  {
    static_assert(is_sentinel::value, "T is not a Sentinel<Iterator>");
    return 1;
  }
};

template <typename T, typename Iterator>
constexpr auto is_sentinel_v = is_sentinel<T, Iterator>::value;
}

template <typename T,
          typename Iterator,
          typename = std::enable_if_t<concepts::is_sentinel<T, Iterator>::value>>
using Sentinel = T;
}
}
