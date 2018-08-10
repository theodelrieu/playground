#pragma once

#include <utility>

namespace mgs
{
inline namespace v1
{
namespace adapters
{
namespace aliases
{
namespace member_functions
{
template <typename T, typename... Args>
using seek_forward =
    decltype(std::declval<T>().seek_forward(std::declval<Args>()...));
}
}
namespace member_functions_aliases = aliases::member_functions;
}
}
}
