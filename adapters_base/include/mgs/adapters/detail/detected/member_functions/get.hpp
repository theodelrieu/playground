#pragma once

#include <utility>

namespace mgs
{
inline namespace v1
{
namespace adapters
{
namespace detail
{
namespace detected
{
namespace member_functions
{
template <typename T, typename... Args>
using get = decltype(std::declval<T>().get(std::declval<Args>()...));
}
}
}
}
}
}
