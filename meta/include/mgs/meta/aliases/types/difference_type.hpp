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
using difference_type = typename T::difference_type;
}
}
namespace type_aliases = aliases::types;
}
}
}
