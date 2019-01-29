#pragma once

#include <utility>

namespace mgs
{
inline namespace v1
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
using get_iterator = decltype(std::declval<T>().get_iterator(std::declval<Args>()...));
}
}
}
}
}
}