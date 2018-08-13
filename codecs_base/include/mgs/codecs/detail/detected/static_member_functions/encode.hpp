#pragma once

#include <utility>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace detail
{
namespace detected
{
namespace static_member_functions
{
template <typename T, typename Ret, typename... Args>
using encode = decltype(T::template encode<Ret>(std::declval<Args>()...));
}
}
}
}
}
}
