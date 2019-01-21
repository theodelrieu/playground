#pragma once

#include <utility>

namespace mgs
{
inline namespace v1
{
namespace ranges
{
namespace detail
{
namespace detected
{
namespace member_functions
{
template <typename T, typename... Args>
using get_sentinel =
    decltype(std::declval<T>().get_sentinel(std::declval<Args>()...));
}
}
}
}
}
}
