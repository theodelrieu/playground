#pragma once

#include <utility>

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace detected
{
namespace member_functions
{
template <typename T, typename... Args>
using resize = decltype(std::declval<T>().resize(std::declval<Args>()...));
}
}
}
}
}
