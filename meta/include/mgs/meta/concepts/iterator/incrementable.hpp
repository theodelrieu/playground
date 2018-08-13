#pragma once

#include <type_traits>

#include <mgs/meta/concepts/iterator/weakly_incrementable.hpp>
#include <mgs/meta/concepts/object/regular.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/post_increment.hpp>

// https://en.cppreference.com/w/cpp/experimental/ranges/iterator/Incrementable

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
struct is_incrementable
  : std::integral_constant<
        bool,
        object::is_regular<T>::value &&
            is_detected_exact<T, detected::operators::post_increment, T&>::
                value &&
            is_weakly_incrementable<T>::value>
{
};
}
};
}
}
}
