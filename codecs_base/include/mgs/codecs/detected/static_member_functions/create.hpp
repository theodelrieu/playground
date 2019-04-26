#pragma once

#include <utility>

namespace mgs
{
namespace codecs
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
