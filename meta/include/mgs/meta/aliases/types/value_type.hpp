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
using value_type = typename T::value_type;
}
}
namespace type_aliases = aliases::types;
}
}
}
