#pragma once

#include <type_traits>
#include <utility>

#include <mgs/meta/call_std/end.hpp>
#include <mgs/meta/concepts/range.hpp>

namespace mgs
{
inline namespace v1
{
namespace meta
{
template <typename R, typename = std::enable_if_t<concepts::is_range<R>::value>>
using sentinel_t = result_of_end<decltype(std::declval<R&>())>;
}
}
}
