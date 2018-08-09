#pragma once

#include <utility>

namespace mgs
{
inline namespace v1
{
namespace binary_to_text
{
namespace aliases
{
namespace static_member_functions
{
template <typename T, typename... Args>
using find_char = decltype(T::find_char(std::declval<Args>()...));
}
}
namespace static_member_function_aliases = aliases::static_member_functions;
}
}
}
