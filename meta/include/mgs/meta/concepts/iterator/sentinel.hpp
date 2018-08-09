#pragma once

#include <type_traits>

#include <mgs/meta/concepts/comparison/weakly_equality_comparable_with.hpp>
#include <mgs/meta/concepts/iterator/iterator.hpp>
#include <mgs/meta/concepts/object/semiregular.hpp>

// http://en.cppreference.com/w/cpp/experimental/ranges/iterator/Sentinel

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace concepts
{
namespace iterator
{
template <typename T, typename Iterator>
struct is_sentinel
  : std::integral_constant<
        bool,
        object::is_semiregular<T>::value && is_iterator<Iterator>::value &&
            comparison::is_weakly_equality_comparable_with<T, Iterator>::value>
{
};
}
}
namespace iterator_concepts = concepts::iterator;
}
}
}
