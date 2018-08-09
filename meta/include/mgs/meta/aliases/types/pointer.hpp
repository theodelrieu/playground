#pragma once

namespace mgs
{
inline namespace v1
{
namespace meta
{
namespace aliases
{
namespace types
{
template <typename T>
using pointer = typename T::pointer;
}
}
namespace type_aliases = aliases::types;
}
}
}
