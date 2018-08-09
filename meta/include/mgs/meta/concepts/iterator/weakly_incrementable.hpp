#pragma once

#include <type_traits>

#include <mgs/meta/aliases/operators/post_increment.hpp>
#include <mgs/meta/aliases/operators/pre_increment.hpp>
#include <mgs/meta/concepts/object/semiregular.hpp>
#include <mgs/meta/detected.hpp>

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
            is_detected_exact<T&, aliases::operators::pre_increment, T&>::
                value &&
            is_detected<aliases::operators::post_increment, T&>::value>
{
};
}
}
namespace iterator_concepts = concepts::iterator;
}
}
}
