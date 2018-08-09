#pragma once

#include <iterator>
#include <type_traits>

#include <mgs/meta/aliases/operators/dereference.hpp>
#include <mgs/meta/aliases/operators/pre_increment.hpp>
#include <mgs/meta/concepts/iterator/iterator_traits.hpp>
#include <mgs/meta/concepts/iterator/weakly_incrementable.hpp>
#include <mgs/meta/concepts/object/semiregular.hpp>
#include <mgs/meta/detected.hpp>

// http://en.cppreference.com/w/cpp/concept/Iterator

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
template <typename T>
struct is_iterator
  : std::integral_constant<
        bool,
        is_iterator_traits<std::iterator_traits<T>>::value &&
            is_weakly_incrementable<T>::value &&
            is_detected<aliases::operators::dereference, T&>::value>
{
};
}
}
namespace iterator_concepts = concepts::iterator;
}
}
}
