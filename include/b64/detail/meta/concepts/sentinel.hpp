#pragma once

#include <type_traits>

#include <b64/detail/meta/concepts/iterator.hpp>
#include <b64/detail/meta/concepts/semiregular.hpp>
#include <b64/detail/meta/concepts/weakly_equality_comparable_with.hpp>

// http://en.cppreference.com/w/cpp/experimental/ranges/iterator/Sentinel
namespace b64
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
