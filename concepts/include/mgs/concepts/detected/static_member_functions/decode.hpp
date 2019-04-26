#pragma once

#include <utility>

namespace mgs
{
namespace concepts
{
namespace detected
{
namespace static_member_functions
{
template <typename T, typename... Args>
using decode = decltype(T::decode(std::declval<Args>()...));

template <typename T, typename Ret, typename... Args>
using decode_tpl = decltype(T::template decode<Ret>(std::declval<Args>()...));
}
}
}
}
