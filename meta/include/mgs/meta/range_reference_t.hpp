#pragma once

#include <type_traits>

#include <mgs/meta/iter_reference_t.hpp>
#include <mgs/meta/iterator_t.hpp>

namespace mgs
{
inline namespace v1
{
namespace meta
{
template <typename R>
using range_value_t = iter_reference_t<iterator_t<R>>;
}
}
}
