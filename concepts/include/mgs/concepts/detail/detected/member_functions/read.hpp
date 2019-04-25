#pragma once

#include <utility>

namespace mgs
{
namespace concepts
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
