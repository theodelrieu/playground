#pragma once

#include <type_traits>

#include <mgs/meta/iterator_t.hpp>
#include <mgs/meta/iter_value_t.hpp>

namespace mgs
{
inline namespace v1
{
namespace meta
{
template <typename R>
using range_value_t = iter_value_t<iterator_t<R>>;
}
}
}
