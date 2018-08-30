#pragma once

#include <type_traits>

#include <mgs/meta/concepts/object/semiregular.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/post_increment.hpp>
#include <mgs/meta/detected/operators/pre_increment.hpp>

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
struct is_weakly_incrementable
  : std::integral_constant<
        bool,
        object::is_semiregular<T>::value &&
            is_detected_exact<T&, detected::operators::pre_increment, T&>::
                value &&
            is_detected<detected::operators::post_increment, T&>::value>
{
};

template <typename T,
          typename = std::enable_if_t<is_weakly_incrementable<T>::value>>
using WeaklyIncrementable = T;
}
};
}
}
}
