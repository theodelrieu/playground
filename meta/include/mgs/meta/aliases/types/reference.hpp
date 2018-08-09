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
using reference = typename T::reference;
}
}
namespace type_aliases = aliases::types;
}
}
}
