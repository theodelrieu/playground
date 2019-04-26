#pragma once

#include <utility>

namespace mgs
{
namespace detected
{
namespace static_member_functions
{
template <typename T, typename... Args>
using encode = decltype(T::encode(std::declval<Args>()...));

template <typename T, typename Ret, typename... Args>
using encode_tpl = decltype(T::template encode<Ret>(std::declval<Args>()...));
}
}
}
