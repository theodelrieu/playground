#pragma once

namespace mgs
{
inline namespace v1
{
namespace adapters
{
namespace aliases
{
namespace types
{
template <typename T>
using underlying_sentinel = typename T::underlying_sentinel;
}
}
namespace type_aliases = aliases::types;
}
}
}
