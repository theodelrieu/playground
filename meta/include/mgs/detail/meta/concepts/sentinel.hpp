#pragma once

#include <type_traits>

#include <mgs/detail/meta/concepts/iterator.hpp>
#include <mgs/detail/meta/concepts/semiregular.hpp>
#include <mgs/detail/meta/concepts/weakly_equality_comparable_with.hpp>

// http://en.cppreference.com/w/cpp/experimental/ranges/iterator/Sentinel
namespace mgs
{
namespace detail
{
template <typename T, typename Iterator>
struct is_sentinel
  : std::integral_constant<
        bool,
        is_semiregular<T>::value && is_iterator<Iterator>::value &&
            is_weakly_equality_comparable_with<T, Iterator>::value>
{
};
}
}
