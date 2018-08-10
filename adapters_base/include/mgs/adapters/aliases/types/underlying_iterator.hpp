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
using underlying_iterator = typename T::underlying_iterator;
}
}
namespace type_aliases = aliases::types;
}
}
}
