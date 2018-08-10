#pragma once

#include <mgs/meta/detected.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace aliases
{
namespace static_member_functions
{
template <typename T, typename Ret, typename... Args>
using encode = decltype(T::template encode<Ret>(std::declval<Args>()...));
}
}
namespace static_member_function_aliases = aliases::static_member_functions;
}
}
}
