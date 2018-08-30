#pragma once

#include <iterator>
#include <type_traits>

#include <mgs/meta/concepts/iterator/iterator_traits.hpp>
#include <mgs/meta/concepts/iterator/weakly_incrementable.hpp>
#include <mgs/meta/concepts/object/semiregular.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/dereference.hpp>
#include <mgs/meta/detected/operators/pre_increment.hpp>

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
            is_detected<detected::operators::dereference, T&>::value>
{
};

template <typename T, typename = std::enable_if_t<is_iterator<T>::value>>
using Iterator = T;
}
};
}
}
}
