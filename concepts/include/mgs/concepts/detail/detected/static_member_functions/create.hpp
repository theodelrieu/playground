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
namespace static_member_functions
{
template <typename T, typename... Args>
using create = decltype(T::create(std::declval<Args>()...));
}
}
}
}
}
