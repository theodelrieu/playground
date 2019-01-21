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
using read = decltype(std::declval<T>().read(std::declval<Args>()...));
}
}
}
}
}
}

