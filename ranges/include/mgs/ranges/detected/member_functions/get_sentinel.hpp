#pragma once

#include <utility>

namespace mgs
{
namespace ranges
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
